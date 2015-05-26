cmake_minimum_required(VERSION 2.8.8)
project(arduino_mock_builder C CXX)
include(ExternalProject)

ExternalProject_Add(the-tentacle
    URL https://github.com/octoblu/the-tentacle/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/the-tentacle
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(the-tentacle source_dir)
set(TENTACLE_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)
