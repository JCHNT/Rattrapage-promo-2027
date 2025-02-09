add_executable(babel_server
    src/ClientSession.cpp
    src/Server.cpp
    src/main_server.cpp
)

find_package(asio REQUIRED CONFIG)

target_include_directories(babel_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(babel_server PRIVATE
    asio::asio
)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    target_link_libraries(babel_server PRIVATE ws2_32 mswsock)
else()
    target_link_libraries(babel_server PRIVATE pthread)
endif()
