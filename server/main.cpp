#include "Server.hpp"
#include <asio.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 84;
    }
    try {
        asio::io_context io_context;
        unsigned short port = std::atoi(argv[1]);
        VoiceServer server(io_context, port);
        server.startAccept();
        std::cout << "Voice Server running on port " << port << std::endl;
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}
