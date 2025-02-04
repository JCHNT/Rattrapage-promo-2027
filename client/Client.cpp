#include "Client.hpp"
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>
#include <iostream>

TcpClient::TcpClient(asio::io_context& io_context, const std::string& host, const std::string& port)
    : io_context_(io_context),
      resolver_(io_context),
      socket_(io_context),
      host_(host),
      port_(port) {}

void TcpClient::connect() {
    asio::ip::tcp::resolver::results_type endpoints = resolver_.resolve(host_, port_);
    asio::async_connect(socket_, endpoints,
        [this](std::error_code ec, asio::ip::tcp::endpoint) {
            if (!ec) {
                doRead();
            } else {
                std::cerr << "Connect failed: " << ec.message() << "\n";
            }
        });
}

void TcpClient::sendMessage(const std::string& message) {
    asio::post(socket_.get_executor(),
        [this, message]() {
            std::ostream os(&write_buffer_);
            os << message;
            doWrite();
        });
}

void TcpClient::doWrite() {
    if (write_buffer_.size() > 0) {
        asio::async_write(socket_, write_buffer_,
            [this](std::error_code ec, std::size_t length) {
                if (!ec) {
                    write_buffer_.consume(length);
                    if (write_buffer_.size() > 0) {
                        doWrite();
                    }
                } else {
                    std::cerr << "Write failed: " << ec.message() << "\n";
                }
            });
    }
}

void TcpClient::doRead() {
    asio::async_read_until(socket_, read_buffer_, '\n',
        [this](std::error_code ec, std::size_t length) {
            if (!ec) {
                std::istream is(&read_buffer_);
                std::string line;
                std::getline(is, line);
                std::cout << "Received: " << line << "\n";
                doRead();
            } else {
                std::cerr << "Read failed: " << ec.message() << "\n";
            }
        });
}