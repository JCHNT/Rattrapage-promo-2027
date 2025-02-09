#include "../include/Server.hpp"
#include <asio.hpp>
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> get_local_ips() {
    std::vector<std::string> ips;
    try {
        asio::io_context io_context;
        std::string hostname = asio::ip::host_name();
        asio::ip::tcp::resolver resolver(io_context);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(hostname, "");
        for (const auto& entry : endpoints) {
            asio::ip::address addr = entry.endpoint().address();
            if (!addr.is_loopback()) {
                ips.push_back(addr.to_string());
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la récupération des IP locales : " << e.what() << std::endl;
    }
    return ips;
}

Server::Server(asio::io_context& io_context, unsigned short port)
    : acceptor_(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
{
    try {
        asio::ip::tcp::endpoint localEndpoint = acceptor_.local_endpoint();
        std::cout << "Serveur en écoute sur " 
                  << localEndpoint.address().to_string() 
                  << ":" << localEndpoint.port() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors de la récupération de l'endpoint local : " << e.what() << std::endl;
    }

    std::vector<std::string> ips = get_local_ips();
    if (!ips.empty()) {
        std::cout << "Adresses IP locales disponibles :" << std::endl;
        for (const auto& ip : ips) {
            std::cout << "  " << ip << std::endl;
        }
    } else {
        std::cout << "Aucune adresse IP locale trouvée hors loopback." << std::endl;
    }

    start_accept();
}

void Server::start_accept() {
    acceptor_.async_accept([this](std::error_code ec, asio::ip::tcp::socket socket) {
        if (!ec) {
            auto session = std::make_shared<ClientSession>(std::move(socket));

            session->set_on_disconnect([this](ClientSession::Ptr s) {
                std::cout << "Un client s'est déconnecté. Fin de l'appel.\n";
                if (session1_ && session2_) {
                    session1_->close();
                    session2_->close();
                    session1_.reset();
                    session2_.reset();
                }
            });

            if (!session1_) {
                session1_ = session;
                std::cout << "Client 1 connecté.\n";
                session1_->start([this](const std::vector<char>& data, ClientSession::Ptr /*sender*/) {
                    if (session2_) {
                        session2_->do_write(data);
                    }
                });
            } else if (!session2_) {
                session2_ = session;
                std::cout << "Client 2 connecté.\n";
                session2_->start([this](const std::vector<char>& data, ClientSession::Ptr /*sender*/) {
                    if (session1_) {
                        session1_->do_write(data);
                    }
                });
            } else {
                std::cerr << "Connexion rejetée : déjà deux clients connectés.\n";
                session->close();
            }
        } else {
            std::cerr << "Erreur d'acceptation : " << ec.message() << "\n";
        }
        if (!session1_ || !session2_) {
            start_accept();
        }
    });
}
