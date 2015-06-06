#include <StandardCplusplus.h>
#include <tentacle.h>

#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "protocols/microblu.pb.h"

#include <tentacle-pseudopod.h>
#include "Stream.h"


#include "Arduino.h"

tentacle::Pseudopod *pseudopod;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pseudopod = new tentacle::Pseudopod(Serial, Serial);
}

void loop() {
  uint8_t buffer[20];
  std::vector<tentacle::Pin> pins;
  pins.push_back(tentacle::Pin(5, 0, 0));
  pins.push_back(tentacle::Pin(8, 0, 1));

  unsigned int messageSize = pseudopod->writeStateMessage(pins);
}
