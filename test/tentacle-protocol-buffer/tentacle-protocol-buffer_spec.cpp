#include <limits.h>
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"

#define TOTAL_PINS 8
#include "tentacle-protocol-buffer.hpp"

using ::testing::Return;
using namespace tentacle;

TEST(TentacleProtoBufTest, getStateMessage_1) {
  Tentacle tentacle;
  TentacleProtoBuf::getStateMessage(tentacle);
}
