set(CMAKE_CXX_FLAGS "-std=c++11")
message ("building tests for the TERRIFYING TENTACLE's TERRIFYING PSEUDOPODS")

file(GLOB SRCS *.cpp *.c )

add_executable(tentacle-pseudopod-test ${SRCS})

linkExternalProject(tentacle-pseudopod-test "gtest")
linkExternalProject(tentacle-pseudopod-test "gmock")
linkExternalProject(tentacle-pseudopod-test "arduino-mock")
linkExternalProject(tentacle-pseudopod-test "arduino-nanopb")
linkExternalProject(tentacle-pseudopod-test "tentacle")
linkExternalProject(tentacle-pseudopod-test "tentacle-pseudopod")


target_link_libraries(tentacle-pseudopod-test
    ${GTEST_LIB_DIR}/libgtest_main.a
)

add_test(tentacle-pseudopod tentacle-pseudopod-test)
