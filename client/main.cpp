#include "Client.hpp"
#include <asio.hpp>
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
        return 1;
    }

    asio::io_context io_context;

    TcpClient client(io_context, argv[1], argv[2]);

    client.connect();

    std::thread t([&io_context]() {
        io_context.run();
    });

    std::string input;
    while (std::getline(std::cin, input)) {
        client.sendMessage(input + "\n");
    }

    io_context.stop();
    t.join();

    return 0;
}
