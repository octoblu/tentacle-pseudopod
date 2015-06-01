cmake_minimum_required(VERSION 2.8.8)
project(arduino_nanopb C CXX)
include(ExternalProject)

ExternalProject_Add(nanopb
    URL https://github.com/octoblu/arduino-nanopb/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nanopb
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(nanopb source_dir)
set_property(GLOBAL PROPERTY NANOPB_INCLUDE_DIRS ${source_dir}/include)

ExternalProject_Get_Property(nanopb binary_dir)
set_property(GLOBAL PROPERTY NANOPB_LIBS_DIR ${binary_dir})
