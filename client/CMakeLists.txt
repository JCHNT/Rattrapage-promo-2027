
find_package(portaudio REQUIRED CONFIG)

find_package(Opus REQUIRED CONFIG)

find_package(asio REQUIRED CONFIG)

add_executable(babel_client
    src/AudioStream.cpp
    src/OpusCodec.cpp
    src/NetworkClient.cpp
    src/main_client.cpp
)

target_include_directories(babel_client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(babel_client PRIVATE
    portaudio::portaudio
    Opus::opus
    asio::asio
)

if(APPLE)
    find_library(AUDIO_TOOLBOX AudioToolbox)
    find_library(CORE_AUDIO CoreAudio)
    target_link_libraries(babel_client PRIVATE ${AUDIO_TOOLBOX} ${CORE_AUDIO})
elseif(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    target_link_libraries(babel_client PRIVATE winmm ws2_32)
else()
    target_link_libraries(babel_client PRIVATE pthread asound)
endif()
