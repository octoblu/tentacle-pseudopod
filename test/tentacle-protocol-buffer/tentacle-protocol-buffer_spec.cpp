#include <limits.h>
#include <vector>

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "tentacle-protocol-buffer.hpp"

using namespace std;
using namespace testing;
using namespace tentacle;

TEST(TentacleProtoBufTest, getStateMessage_1) {
  uint8_t buffer[128];
  vector<tentacle::Pin> pins;

  TentacleProtoBuf tentacleProtoBuf(buffer, 128);
  tentacleProtoBuf.writeStateMessage(pins);
  vector<tentacle::Pin> pins2 = tentacleProtoBuf.readStateMessage();
  EXPECT_EQ(pins2.size(), 1);
}
