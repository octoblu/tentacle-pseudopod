cmake_minimum_required (VERSION 3.2)
project (tentacleProtoBuf)

include_directories(lib/nanopb)
include_directories(lib/tentacle)
include_directories(TENTACLE_INCLUDE_DIRS)

enable_testing()
add_subdirectory(test)
