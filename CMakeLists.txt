cmake_minimum_required (VERSION 2.8)
project (tentacle-protocol-buffer)
SET(CMAKE_CXX_FLAGS "-std=c++0x")
add_subdirectory(lib/tentacle)

message("tentacle include directories: ${TENTACLE_INCLUDE_DIRS}")
# include_directories(src/include src/protocols lib/nanopb ${TENTACLE_INCLUDE_DIRS})
#
# add_subdirectory(src)
# enable_testing()
# add_subdirectory(test)
