set(CMAKE_CXX_FLAGS "-std=c++11")
message ("building tests for the TERRIFYING TENTACLE (protocol buffer)")

file(GLOB SRCS *.cpp *.hpp *.c *.h)

add_executable(tentacle-proto-buf-test ${SRCS})

linkExternalProject(tentacle-proto-buf-test "gtest")
linkExternalProject(tentacle-proto-buf-test "gmock")
linkExternalProject(tentacle-proto-buf-test "arduino-mock")
linkExternalProject(tentacle-proto-buf-test "arduino-nanopb")
linkExternalProject(tentacle-proto-buf-test "tentacle")
linkExternalProject(tentacle-proto-buf-test "tentacle-proto-buf")


target_link_libraries(
    ${GTEST_LIB_DIR}/libgtest_main.a
)

add_test(tentacle-proto-buf tentacle-proto-buf-test)
