cmake_minimum_required(VERSION 2.8)
project (tentacleProtoBuf)
set(CMAKE_CXX_FLAGS "-std=c++98")
include(ExternalProject)

find_package(Threads REQUIRED)

enable_testing()

#Test libs
ExternalProject_Add(gtest
    URL https://googletest.googlecode.com/files/gtest-1.7.0.zip
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Add(gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Add(arduino_mock
    GIT_REPOSITORY git@github.com:octoblu/arduino-mock
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)


#runtime libs
ExternalProject_Add(arduino-nanopb
    GIT_REPOSITORY git@github.com:octoblu/arduino-nanopb
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

ExternalProject_Add(tentacle
    GIT_REPOSITORY git@github.com:octoblu/the-tentacle
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
)

file(GLOB SRCS *.cpp *.hpp *.c *.h)
add_executable(tentacleProtoBufSpec ${SRCS})

add_library(tentacleProtoBuf tentacle-protocol-buffer.cpp microblu.pb.c)

add_test(tentacleProtoBuf tentacleProtoBufSpec)
