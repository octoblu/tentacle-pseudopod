message ("building tests for the TERRIFYING TENTACLE")
include_directories(
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${ARDUINO_MOCK_INCLUDE_DIRS}
)

file(GLOB SRCS *.cpp *.hpp)
add_executable(tentacleProtocolBufferSpec ${SRCS})

message("tentacle build files are: ${TENTACLE_PROTO_BUF_LIBS_INIT}")
target_link_libraries(tentacleProtocolBufferSpec
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${ARDUINO_MOCK_LIBS_DIR}/libarduino_mock.a
    ${GMOCK_LIBS_DIR}/libgmock.a
    ${TENTACLE_PROTO_BUF_LIBS_DIR}
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(tentacleProtocolBufferSpec gtest )
add_test(tentacleProtolBuffer tentacleProtocolBufferSpec)
