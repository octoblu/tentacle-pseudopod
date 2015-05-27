message ("building tests for the TERRIFYING TENTACLE")
get_property(TENTACLE_INCLUDE_DIRS GLOBAL PROPERTY TENTACLE_INCLUDE_DIRS)
get_property(ARDUINO_MOCK_INCLUDE_DIRS GLOBAL PROPERTY ARDUINO_MOCK_INCLUDE_DIRS)
include_directories(
  ../..
  ${GTEST_INCLUDE_DIRS}
  ${GMOCK_INCLUDE_DIRS}
  ${ARDUINO_MOCK_INCLUDE_DIRS}
  ${TENTACLE_INCLUDE_DIRS}
)

file(GLOB SRCS *.cpp *.hpp)
add_executable(tentacleProtoBufSpec ${SRCS})


message("tentacle header files are: ${TENTACLE_INCLUDE_DIRS}")
target_link_libraries(tentacleProtoBufSpec
    tentacleProtoBuf
    ${GTEST_LIBS_DIR}/libgtest.a
    ${GTEST_LIBS_DIR}/libgtest_main.a
    ${ARDUINO_MOCK_LIBS_DIR}/libarduino_mock.a
    ${GMOCK_LIBS_DIR}/libgmock.a
    ${CMAKE_THREAD_LIBS_INIT}
)
add_dependencies(tentacleProtoBufSpec gtest)
add_test(tentacleProBuf tentacleProtoBufSpec)
