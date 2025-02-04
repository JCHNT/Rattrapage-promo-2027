#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include <array>

using asio::ip::tcp;

class VoiceSession : public std::enable_shared_from_this<VoiceSession> {
public:
    VoiceSession(tcp::socket socket, VoiceServer *server)
        : socket(std::move(socket)), server(server) {}

    void start() {
        doRead();
    }

    void send(const std::string &message) {
        auto self(shared_from_this());
        asio::async_write(socket, asio::buffer(message),
            [this, self](const asio::error_code &ec, std::size_t /*length*/) {
                if(ec) {
                    std::cerr << "Send error: " << ec.message() << std::endl;
                }
            });
    }

private:
    void doRead() {
        auto self(shared_from_this());
        socket.async_read_some(asio::buffer(data),
            [this, self](const asio::error_code &ec, std::size_t length) {
                if(!ec)
                {
                    std::string message(data.data(), length);
                    std::cout << "Received: " << message << std::endl;
                    server->broadcast(message, self);
                    doRead();
                }
                else
                {
                    std::cout << "Session ended: " << ec.message() << std::endl;
                    server->removeSession(self);
                }
            });
    }

    tcp::socket socket;
    std::array<char, 4096> data;
    VoiceServer *server;
};

VoiceServer::VoiceServer(asio::io_context &io_context, unsigned short port)
    : io_context(io_context),
      acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
}

void VoiceServer::startAccept() {
    doAccept();
}

void VoiceServer::doAccept() {
    acceptor.async_accept(
        [this](const asio::error_code &ec, tcp::socket socket) {
            if(!ec)
            {
                auto session = std::make_shared<VoiceSession>(std::move(socket), this);
                {
                    std::lock_guard<std::mutex> lock(sessionsMutex);
                    sessions.push_back(session);
                }
                std::cout << "New client connected." << std::endl;
                session->start();
            }
            doAccept();
        }
    );
}

void VoiceServer::broadcast(const std::string &message, std::shared_ptr<VoiceSession> sender) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    for(auto &session : sessions)
    {
        if(session != sender)
        {
            session->send(message);
        }
    }
}

void VoiceServer::removeSession(std::shared_ptr<VoiceSession> session) {
    std::lock_guard<std::mutex> lock(sessionsMutex);
    sessions.erase(std::remove(sessions.begin(), sessions.end(), session), sessions.end());
}
