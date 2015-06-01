cmake_minimum_required(VERSION 2.8)

project (tentacleProtoBuf)
set(CMAKE_CXX_FLAGS "-std=c++98")

add_subdirectory(lib)

# get_property(ARDUINO_MOCK_INCLUDE_DIRS GLOBAL PROPERTY ARDUINO_MOCK_INCLUDE_DIRS)
get_property(GMOCK_INCLUDE_DIRS GLOBAL PROPERTY GMOCK_INCLUDE_DIRS)
get_property(GTEST_INCLUDE_DIRS GLOBAL PROPERTY GTEST_INCLUDE_DIRS)
# get_property(NANOPB_INCLUDE_DIRS GLOBAL PROPERTY NANOPB_INCLUDE_DIRS)
# get_property(ARDUINO_NANOPB_INCLUDE_DIRS GLOBAL PROPERTY ARDUINO_NANOPB_INCLUDE_DIRS)
# get_property(TENTACLE_INCLUDE_DIRS GLOBAL PROPERTY TENTACLE_INCLUDE_DIRS)
# get_property(TENTACLE_LIBS_DIR GLOBAL PROPERTY TENTACLE_LIBS_DIR)

include_directories(
  ${ARDUINO_MOCK_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${TENTACLE_INCLUDE_DIRS}
  ${NANOPB_INCLUDE_DIRS}
  ${ARDUINO_NANOPB_INCLUDE_DIRS}
  lib/tentacle
)

add_library(tentacleProtoBuf tentacle-protocol-buffer.cpp microblu.pb.c)

target_link_libraries(
  tentacleProtoBuf
)

add_dependencies(tentacleProtoBuf tentacle)

enable_testing()
add_subdirectory(test)
