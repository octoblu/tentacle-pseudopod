message ("building tests for the TERRIFYING TENTACLE PROTOCOL BUFFER")

include_directories(
  ../..
  ${TENTACLE_PROTO_BUF_INCLUDE_DIRS}
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${ARDUINO_MOCK_INCLUDE_DIRS}
  ${NANOPB_INCLUDE_DIRS}
  ${TENTACLE_INCLUDE_DIRS}
)

file(GLOB SRCS *.cpp *.hpp *.c *.h)
message("I think srcs are ${SRCS}")
add_executable(tentacleProtoBufSpec ${SRCS})

target_link_libraries(tentacleProtoBufSpec
    tentacleProtoBuf
    nanopb
    ${TENTACLE_LIBS_DIR}/libtentacle.a
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${ARDUINO_MOCK_LIBS_DIR}/libarduino_mock.a
    ${ARDUINO_MOCK_LIBS_DIR}/libarduino_mock.a
    ${GMOCK_LIBS_DIR}/libgmock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(tentacleProtoBufSpec gtest)
add_test(tentacleProtoBuf tentacleProtoBufSpec)
