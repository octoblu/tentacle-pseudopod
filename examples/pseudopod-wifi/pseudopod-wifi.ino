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


char ssid[] = "octoblu-guest";
char password[] = "octoblu1";
IPAddress server(172,16,42,44);

/*char ssid[] = "ROBOT-WASTELAND";
char password[] = "lemonade";
IPAddress server(192,168,0,112);*/


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

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void loop() {
  if (!conn.connected()) {
    Serial.print(F("I wasn't connected!"));
    Serial.flush();
    conn.stop();
    connectToServer();
  }
  Serial.print(F("Number of Pins:"));
  Serial.println(tentacle.getNumPins());
  Serial.println(F("writing"));
  Serial.flush();
  sendData();
  readData();
}

void sendData() {
  int written = pseudopod.sendPins( tentacle.getValue());
  Serial.print(written);
  Serial.println(F(" bytes written."));
  Serial.print(freeRam());
  Serial.println(F(" bytes free"));
  Serial.flush();
  delay(2000);
}

void readData() {
  while (conn.available()) {
    delay(2000);
    Serial.println(F("DATA WAS AVAILABLE!"));
    Serial.flush();
    TentacleMessage message = pseudopod.getMessage();
    std::vector<Pin> pins;

    switch(message.getTopic()) {

      case TentacleMessage::action:
        pins = tentacle.getValue(message.getPins());
        pseudopod.sendPins(pins);
      break;

      case TentacleMessage::config:
        tentacle.configurePins(message.getPins());
      break;

    }
    Serial.print(F("How many pins did we get?"));
    Serial.println(message.getPins().size());
  }
}

void connectToServer() {
  Serial.println(F("Connecting to the server."));
  Serial.flush();
  while(!conn.connect(server, port)) {
    Serial.println(F("Can't connect to the server."));
    Serial.flush();
    conn.stop();
    delay(1000);
  }
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

  connectToServer();
}

void softReset() {
  asm volatile ("  jmp 0");
}
