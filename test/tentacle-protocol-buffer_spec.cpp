#include <limits.h>
#include <vector>

#include "gtest/gtest.h"
#include "Stream.h"
#include "BufferStream.hpp"

#include "tentacle-pseudopod.h"
#include <stdio.h>

using namespace std;

TEST(PseudopodTest, writeStateMessage_1) {
  int length = 128;
  uint8_t ibuffer[length];
  uint8_t obuffer[length];

  BufferStream istream(ibuffer, length);
  BufferStream ostream(obuffer, length);

  vector<Pin> pins;
  pins.push_back(Pin(4, Pin::digitalRead, 1));
  pins.push_back(Pin(40, Pin::analogWrite, 0));

  Pseudopod pseudopod(istream, ostream);
  cout << "writingMessage" << endl;
  pseudopod.sendPins(pins);

  cout << "sending to input stream" << endl;
  istream.write(obuffer, ostream.available());
  vector<Pin> pins2 = pseudopod.getMessage().getPins();

  EXPECT_EQ(pins2.size(), 2);
}

TEST(PseudopodTest, writeStateMessage_2) {
  int length = 128;
  uint8_t ibuffer[length];
  uint8_t obuffer[length];

  BufferStream istream(ibuffer, length);
  BufferStream ostream(obuffer, length);

  vector<Pin> pins;

  Pseudopod pseudopod(istream, ostream);

  pins.push_back(Pin(5, Pin::digitalRead, 0));
  pins.push_back(Pin(8, Pin::analogRead, 1));

  unsigned int messageSize = pseudopod.sendPins(pins);

  cout << "sending to input stream" << endl;
  istream.write(obuffer, ostream.available());

  vector<Pin> pins2 = pseudopod.getMessage().getPins();
  
  EXPECT_EQ(pins2.size(), 2);
  EXPECT_EQ(pins2[0].getNumber(), pins[0].getNumber());
  EXPECT_EQ(pins2[0].getValue(), pins[0].getValue());

  EXPECT_EQ(pins2[1].getNumber(), pins[1].getNumber());
  EXPECT_EQ(pins2[1].getValue(), pins[1].getValue());
}
