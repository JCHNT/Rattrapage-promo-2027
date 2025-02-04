cmake_minimum_required(VERSION 3.15)
project(VoiceCallProject LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable threading support
find_package(Threads REQUIRED)
if(THREADS_FOUND)
    set(THREAD_LIBRARIES Threads::Threads)
endif()

# --- Configuration Conan ---
include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake OPTIONAL)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake OPTIONAL)

# --- Trouver Asio (via Conan ou système) ---
find_package(asio REQUIRED CONFIG)

# --- Cible Serveur ---
add_executable(babel_server
    server/main.cpp
    server/Server.cpp
    server/Server.hpp
)

# --- Cible Client ---
add_executable(babel_client
    client/main.cpp
    client/Client.cpp
    client/Client.hpp
)

# Link Asio and Threads to the server and client targets
if(DEFINED THREAD_LIBRARIES)
    target_link_libraries(babel_server PRIVATE asio::asio ${THREAD_LIBRARIES})
    target_link_libraries(babel_client PRIVATE asio::asio ${THREAD_LIBRARIES})
else()
    target_link_libraries(babel_server PRIVATE asio::asio)
    target_link_libraries(babel_client PRIVATE asio::asio)
endif()

# Set include directories if needed
target_include_directories(babel_server PRIVATE ${CMAKE_SOURCE_DIR}/server)
target_include_directories(babel_client PRIVATE ${CMAKE_SOURCE_DIR}/client)

# Enable position-independent code (PIC) for shared libraries
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add compile options for better warnings and debugging
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4 /WX)
endif()

# Install targets if needed
install(TARGETS babel_server babel_client DESTINATION bin)