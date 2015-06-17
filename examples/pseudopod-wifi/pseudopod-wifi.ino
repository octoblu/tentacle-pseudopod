#include <SPI.h>
#include <WiFi.h>
#include <StandardCplusplus.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

/*#include "MemoryFree.h"*/

#include "tentacle.h"
#include "tentacle-arduino.h"
#include "tentacle-pseudopod.h"

/*#include "BufferStream.hpp"*/
#include <string>
#include <EEPROM.h>
#include <vector>
#include "pins.hpp"
#define DELAY 10000

/*char ssid[] = "octoblu-guest";
char password[] = "octoblu1";
IPAddress server(172,16,42,4);*/

char ssid[]     = "Robot Outpost";
char password[] = "lemonade";

static const char uuid[]  = "b9944342-b8c7-4ca6-9d3e-074eb4706264";
static const char token[] = "6d1f0dd95bf0dc0beb64ab7252152de6a2c08583";

IPAddress server(192,168,43,30);


#define port 8111

int status = WL_IDLE_STATUS;
WiFiClient conn;
Pseudopod pseudopod(conn, conn);
TentacleArduino tentacle;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up."));
  setupWifi();
}

void loop() {
  if (!conn.connected()) {
    Serial.println(F("No connection!"));
    Serial.flush();
    conn.stop();
    connectToServer();
  }
  sendData();
  readData();
}

void sendData() {
  delay(DELAY);

  Serial.println(F("writing"));
  Serial.flush();
/*
  std::vector<Pin> pins;
  for(int i = 0; i < 20; i++) {
    pins.push_back(Pin(i, Pin::digitalRead, 0));
  }
*/
  size_t written = pseudopod.sendPins(*tentacle.getValue());
  Serial.print(written);
  Serial.println(F(" bytes written."));
  Serial.print(freeRam());
  Serial.println(F(" bytes free"));
  Serial.flush();
}

void readData() {
  while (conn.available()) {
    delay(DELAY);
    Serial.println(F("DATA WAS AVAILABLE!"));
    Serial.flush();
    TentacleMessage message = pseudopod.getMessage();
    std::vector<Pin> msgPins = message.getPins();
    std::vector<Pin> pins;

    switch(message.getTopic()) {

      case TentacleMessage::action:
        pins = *tentacle.getValue(&msgPins);
        pseudopod.sendPins(pins);
      break;

      case TentacleMessage::config:
        tentacle.configurePins(msgPins);
      break;

    }
    Serial.print(F("How many pins did we get?"));
    Serial.println(message.getPins().size());
  }
}

void connectToServer() {
  int connectionAttempts = 0;
  Serial.println(F("Connecting to the server."));
  Serial.flush();
  while(!conn.connect(server, port)) {
    if(connectionAttempts > 10) {
      Serial.println(F("Still can't connect. I must have gone crazy. Rebooting"));
      Serial.flush();

      softReset();
    }
    Serial.println(F("Can't connect to the server."));
    Serial.flush();
    conn.stop();
    delay(1000);
    connectionAttempts++;
  }
  size_t authSize = pseudopod.authenticate(uuid, token);
  Serial.print(authSize);
  Serial.println(F(" bytes written for authentication"));
  Serial.flush();
}

void setupWifi() {
  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to SSID: "));
    Serial.println(ssid);
    Serial.flush();
    status = WiFi.begin(ssid, password);
  }

  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));

  Serial.flush();
}

void softReset() {
  asm volatile ("  jmp 0");
}

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
