#include <limits.h>
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"

#define TOTAL_PINS 8
#include "../../src/tentacle-protocol-buffer.cpp"
// #include "../../src/tentacle.cpp"


using ::testing::Return;
using namespace tentacle;

TEST(TentacleProtoBufTest, Constructor) {
  TentacleProtoBuf tentacleProtoBuf(1);
}
