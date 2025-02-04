cmake_minimum_required(VERSION 3.15)
project(VoiceCallProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Configuration Conan ---
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)

# --- Trouver Asio (via Conan ou système) ---
find_package(asio REQUIRED CONFIG)

# --- Cible Serveur ---
add_executable(babel_server
    server/main.cpp
    server/Server.cpp
    server/Server.hpp
)

target_link_libraries(babel_server PRIVATE asio::asio)