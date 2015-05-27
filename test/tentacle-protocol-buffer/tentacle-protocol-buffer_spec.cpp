#include <limits.h>
#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"

#include "tentacle-protocol-buffer.hpp"

using namespace testing;
using namespace tentacle;

TEST(TentacleProtoBufTest, getStateMessage_1) {
  Tentacle tentacle;
  MicrobluState message = TentacleProtoBuf::getStateMessage(tentacle);
}
