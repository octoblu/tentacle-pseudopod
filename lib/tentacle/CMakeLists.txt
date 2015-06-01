cmake_minimum_required(VERSION 2.8.8)
project(arduino_mock_builder C CXX)
include(ExternalProject)

ExternalProject_Add(tentacle
    URL https://github.com/octoblu/the-tentacle/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(tentacle source_dir)
set_property(GLOBAL PROPERTY TENTACLE_INCLUDE_DIRS ${source_dir}/include)

ExternalProject_Get_Property(tentacle binary_dir)
set_property(GLOBAL PROPERTY TENTACLE_LIBS_DIR ${binary_dir})
