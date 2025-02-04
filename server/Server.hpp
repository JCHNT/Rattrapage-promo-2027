#ifndef VOICESERVER_HPP
#define VOICESERVER_HPP

#include <asio.hpp>
#include <memory>
#include <vector>
#include <mutex>
#include <string>

using asio::ip::tcp;

class VoiceSession; // déclaration anticipée

class VoiceServer {
public:
    VoiceServer(asio::io_context &io_context, unsigned short port);
    void startAccept();
    void broadcast(const std::string &message, std::shared_ptr<VoiceSession> sender);
    void removeSession(std::shared_ptr<VoiceSession> session);

private:
    void doAccept();

    asio::io_context &io_context;
    tcp::acceptor acceptor;
    std::vector<std::shared_ptr<VoiceSession>> sessions;
    std::mutex sessionsMutex;
};

#endif // VOICESERVER_HPP
