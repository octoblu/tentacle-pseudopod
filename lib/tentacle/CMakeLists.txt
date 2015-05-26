cmake_minimum_required(VERSION 2.8.8)
include(ExternalProject)
ExternalProject_Add(tentacle
    URL https://github.com/octoblu/the-tentacle/archive/master.zip
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(tentacle source_dir)
add_library (libtentacle ${source_dir}/src/tentacle.cpp)
target_include_directories (libtentacle PUBLIC libtentacle ${source_dir}/src/include)
