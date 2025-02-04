#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP

#include <asio.hpp>
#include <iostream>
#include <asio/ip/tcp.hpp>
#include <asio/write.hpp>
#include <asio/read_until.hpp>
#include <string>

class TcpClient {
public:
    TcpClient(asio::io_context& io_context, const std::string& host, const std::string& port);

    void connect();
    void sendMessage(const std::string& message);

private:
    void doConnect() {};
    void doWrite(const std::string& message);
    void doRead();

    asio::io_context& io_context_;
    asio::ip::tcp::resolver resolver_;
    asio::ip::tcp::socket socket_;
    asio::streambuf read_buffer_;
    std::string host_;
    std::string port_;
    asio::streambuf write_buffer_;
};

#endif // TCP_CLIENT_HPP
