cmake_minimum_required(VERSION 2.8.8)

message ("building tentacle-protocol-buffers")

file(GLOB SRC
    "*.hpp"
    "*.cpp"
)

include_directories(
  ${TENTACLE_INCLUDE_DIRS}
  include
)

file(GLOB SRCS *.cpp *.hpp)
