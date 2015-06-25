#include <SPI.h>
#include <WiFi.h>
#include "Arduino.h"

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "tentacle-arduino.h"
#include "tentacle-pseudopod.h"

#include <EEPROM.h>

#define DELAY 2000

//octoblu hq
/*char ssid[] = "octoblu-guest";
char password[] = "octoblu1";
IPAddress server(172,16,42,44);*/
#include "wifi-credentials.h"
#define port 8111

static const char uuid[]  = "b9944342-b8c7-4ca6-9d3e-074eb4706264";
static const char token[] = "6d1f0dd95bf0dc0beb64ab7252152de6a2c08583";

int status = WL_IDLE_STATUS;
WiFiClient conn;

TentacleArduino tentacle;
Pseudopod pseudopod(conn, conn, tentacle);

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up."));

  setupWifi();
  connectToServer();
  delay(DELAY);
}

void loop() {
  if (!conn.connected()) {
    conn.stop();
    connectToServer();
  }

  readData();
  pseudopod.sendConfiguredPins();
  Serial.print(F("Free ram: "));
  Serial.print(freeRam());
  Serial.println(F(" bytes"));
  Serial.flush();
}

void readData() {
  delay(DELAY);

  while (conn.available()) {
    Serial.println(F("Received message"));
    Serial.flush();

    if(pseudopod.readMessage() == TentacleMessageTopic_action) {
      delay(DELAY);
      pseudopod.sendPins();
    }
  }

  delay(DELAY);
}

void connectToServer() {
  int connectionAttempts = 0;
  Serial.println(F("Connecting to the server."));
  Serial.flush();

  while(!conn.connect(server, port)) {
    if(connectionAttempts > 5) {
      Serial.println(F("Still can't connect. I must have gone crazy. Rebooting"));
      Serial.flush();

      softReset();
    }
    Serial.println(F("Can't connect to the server."));
    Serial.flush();
    conn.stop();
    delay(DELAY);
    connectionAttempts++;
  }

  size_t authSize = pseudopod.authenticate(uuid, token);
  Serial.print(authSize);
  Serial.println(F(" bytes written for authentication"));

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

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}
