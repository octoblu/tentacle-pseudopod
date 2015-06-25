#include <StandardCplusplus.h>
#include <tentacle.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include <tentacle-pseudopod.h>
#include "BufferStream.hpp"

#include "Arduino.h"
#define length 40
using namespace tentacle;

Pseudopod *pseudopod;
std::PinActions pins;
uint8_t ibuffer[length];
uint8_t obuffer[length];

BufferStream istream(ibuffer, length);
BufferStream ostream(obuffer, length);

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");

  pins.push_back(Pin(4, 0, 1));
  pins.push_back(Pin(40, 0, 0));

  pseudopod = new Pseudopod(istream, ostream);
  Serial.println("writingMessage()");
  pseudopod->writeStateMessage(pins);

  Serial.println("sending to input stream");
  istream.write(obuffer, ostream.available());

  std::PinActions pins2 = pseudopod->readStateMessage();

  Serial.print("Pin size: ");
  Serial.print(pins2.size());
  Serial.println(" should be 2.");

}


void loop() {
}
