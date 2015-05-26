message ("building tests for the TERRIFYING TENTACLE")
include_directories(
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${ARDUINO_MOCK_INCLUDE_DIRS}
  ${TENTACLE_INCLUDE_DIRS}
)

file(GLOB SRCS *.cpp *.hpp)
add_executable(tentacleProtocolBufferSpec ${SRCS})
message("Arduino build files are: ${ARDUINO_MOCK_LIBS_DIR}")
target_link_libraries(tentacleProtocolBufferSpec
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${ARDUINO_MOCK_LIBS_DIR}/libarduino_mock.a
    ${GMOCK_LIBS_DIR}/libgmock.a
    ${CMAKE_THREAD_LIBS_INIT}
)

add_dependencies(tentacleProtocolBufferSpec gtest)
add_test(tentacleProtolBuffer tentacleProtocolBufferSpec)
