#include <limits.h>
#include <vector>

#include "gtest/gtest.h"
#include "arduino-mock/Arduino.h"
#include "arduino-mock/Serial.h"
#include "tentacle-protocol-buffer.hpp"

using namespace std;
using namespace testing;
using namespace tentacle;

TEST(TentacleProtoBufTest, writeStateMessage_1) {
  uint8_t buffer[128];
  vector<tentacle::Pin> pins;
  pins.push_back(Pin(4, 1, 5));
  pins.push_back(Pin(40, 1, 15));
  TentacleProtoBuf tentacleProtoBuf(buffer, 128);
  unsigned int messageSize = tentacleProtoBuf.writeStateMessage(pins);
  vector<tentacle::Pin> pins2 = tentacleProtoBuf.readStateMessage(messageSize);

  EXPECT_EQ(pins2.size(), 2);
}
