cmake_minimum_required(VERSION 2.8.8)
include(ExternalProject)
ExternalProject_Add(tentacle
    URL https://github.com/octoblu/the-tentacle/archive/master.zip
    INSTALL_COMMAND ""
)

# Specify include dir
ExternalProject_Get_Property(tentacle source_dir)
set(TENTACLE_INCLUDE_DIRS ${source_dir} PARENT_SCOPE)
message("--------------> tentacle stuff is in ${TENTACLE_INCLUDE_DIRS}")
