#include <SPI.h>
#include <WiFi.h>

#include "Arduino.h"

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

/*#include "MemoryFree.h"*/

#include "tentacle.h"
#include "tentacle-arduino.h"
#include "tentacle-pseudopod.h"



/*#include "BufferStream.hpp"*/
#include <EEPROM.h>
#include "pins.hpp"
#define DELAY 2000

/*char ssid[] = "octoblu-guest";
char password[] = "octoblu1";
IPAddress server(172,16,42,4);*/

char ssid[]     = "Robot Outpost";
char password[] = "lemonade";

static const char uuid[]  = "b9944342-b8c7-4ca6-9d3e-074eb4706264";
static const char token[] = "6d1f0dd95bf0dc0beb64ab7252152de6a2c08583";

IPAddress server(192,168,29,167);


#define port 8111

int status = WL_IDLE_STATUS;
WiFiClient conn;
Pseudopod pseudopod(conn, conn);
TentacleArduino tentacle;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up."));

  printCredentials(1);

  setupWifi();
  connectToServer();
  delay(DELAY);
}

void loop() {
  if (!conn.connected()) {
    conn.stop();
    Serial.println(F("No connection!"));
    Serial.flush();

    connectToServer();

  }
  sendData();
  readData();
  printCredentials(100);
}

void sendData() {
  delay(DELAY);

  Serial.println(F("writing"));
  Serial.flush();

  size_t written = pseudopod.sendPins(tentacle);
  Serial.print(written);
  Serial.println(F(" bytes written."));
  Serial.print(freeRam());
  Serial.println(F(" bytes free"));
  Serial.flush();
}

void readData() {
  delay(DELAY);
  while (conn.available()) {
    Serial.println(F("DATA WAS AVAILABLE!"));
    Serial.flush();
    pseudopod.processMessage(tentacle);
  }
}

void connectToServer() {
  printCredentials(3);

  int connectionAttempts = 0;
  Serial.println(F("Connecting to the server."));
  Serial.flush();
  while(!conn.connect(server, port)) {
    printCredentials(4);
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

  printCredentials(2);

  size_t authSize = pseudopod.authenticate(uuid, token);
  Serial.print(authSize);
  Serial.println(F(" bytes written for authentication"));
  /*Serial.flush();*/
}

void setupWifi() {
  printCredentials(5);
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
  printCredentials(6);
}

void softReset() {
  asm volatile ("  jmp 0");
}

void printCredentials(int marker) {
  Serial.println(marker);

  Serial.print(F("uuid in char[]:\t"));
  Serial.println(uuid);

  Serial.print(F("token in char[]:\t"));
  Serial.println(token);
  Serial.flush();
}

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
