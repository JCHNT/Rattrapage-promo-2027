#ifndef SERVER_HPP
#define SERVER_HPP

#include <asio.hpp>
#include <memory>
#include "ClientSession.hpp"

class Server {
public:
    Server(asio::io_context& io_context, unsigned short port);

private:
    void start_accept();

    asio::ip::tcp::acceptor acceptor_;
    std::shared_ptr<ClientSession> session1_;
    std::shared_ptr<ClientSession> session2_;
    std::shared_ptr<ClientSession> session3_;
};

#endif // SERVER_HPP
