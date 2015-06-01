#define ARDUINO_H
typedef unsigned char byte;
#include <StandardCplusplus.h>
#include <tentacle.h>
#include <tentacle-protocol-buffer.h>
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
  TentacleProtoBuf tentacleProtoBuf(buffer, 20);
  unsigned int messageSize = tentacleProtoBuf.writeStateMessage(pins);
  vector<Pin> pins2 = tentacleProtoBuf.readStateMessage(messageSize);

  Serial.print("Pin 1: ");
  Serial.print(pins2[0].getNumber());
  Serial.print(" ");
  Serial.println(pins2[0].getValue());

  Serial.print("Pin 2: ");
  Serial.print(pins2[1].getNumber());
  Serial.print(" ");
  Serial.println(pins2[1].getValue());

}
