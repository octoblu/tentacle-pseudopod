# cmake_minimum_required(VERSION 2.8.8)
project (tentacleProtoBuf)

add_subdirectory(lib)
add_subdirectory(protocols)
include_directories(lib/nanopb)
include_directories(lib/tentacle)
include_directories(protocols)

get_property(TENTACLE_INCLUDE_DIRS GLOBAL PROPERTY TENTACLE_INCLUDE_DIRS)
message("protobuf thinks the tentacle include dirs are ${TENTACLE_INCLUDE_DIRS}")

add_subdirectory(test)

include_directories(.
  ${TENTACLE_INCLUDE_DIRS}
  )

add_library(tentacleProtoBuf tentacle-protocol-buffer.cpp)

target_link_libraries(
  tentacleProtoBuf
  nanopb
  protocols
)

enable_testing()

add_dependencies(tentacleProtoBuf tentacle)
