typedef unsigned char byte;
#include <StandardCplusplus.h>
#include <tentacle.h>
#include <tentacle-pseudopod.h>
#include "Stream.h"
extern "C" {
  #include <pb_encode.h>
  #include <pb_decode.h>
}

using namespace std;
using namespace tentacle;
Tentacle t;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  uint8_t buffer[20];
  vector<Pin> pins;
  pins.push_back(Pin(5, 0, 0));
  pins.push_back(Pin(8, 0, 1));
  TentacleProtoBuf tentacleProtoBuf(Serial, Serial);
  unsigned int messageSize = tentacleProtoBuf.writeStateMessage(pins);
}
