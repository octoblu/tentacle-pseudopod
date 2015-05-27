cmake_minimum_required(VERSION 2.8.8)
project(arduino_mock_builder C CXX)
include(ExternalProject)

ExternalProject_Add(tentacle
    URL https://github.com/octoblu/the-tentacle/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/tentacle
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(tentacle source_dir)
set(TENTACLE_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)
set_property(GLOBAL PROPERTY TENTACLE_INCLUDE_DIRS ${source_dir})

ExternalProject_Get_Property(tentacle install_dir)
#get_property(ARDUINO_MOCK_INCLUDE_DIRS GLOBAL PROPERTY ARDUINO_MOCK_INCLUDE_DIRS)
#ExternalProject_Get_Property(ARDUINO_MOCK_INCLUDE_DIRS ARDUINO_MOCK_INCLUDE_DIRS)
message("---------------->  I tried to set the arduino mock include dirs to: ${ARDUINO_MOCK_INCLUDE_DIRS}")
message("Wow I have a install_dir of ${install_dir}")
# Specify MainTest's link libraries
ExternalProject_Get_Property(tentacle binary_dir)
set(TENTACLE_LIBS_DIR ${binary_dir}/dist/lib PARENT_SCOPE)
set_property(GLOBAL PROPERTY TENTACLE_LIBS_DIR ${binary_dir}/dist/lib)
