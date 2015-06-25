#include <SPI.h>
#include <WiFi.h>

#include "StandardCplusplus.h"
#include <vector>
#include "Arduino.h"

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "tentacle-message.h"
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
Pseudopod pseudopod(conn, conn);

using namespace std;

void sendData(const vector<Pin>& pins) {
  delay(DELAY);

  Serial.println(F("writing"));
  Serial.flush();

  size_t written = pseudopod.sendPins(pins);
  Serial.print(written);
  Serial.println(F(" bytes written."));
  Serial.print(freeRam());
  Serial.println(F(" bytes free"));
  Serial.flush();
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starting up."));
  printToken(0);

  setupWifi();
  connectToServer();
  delay(DELAY);
}

void loop() {
  printToken(1);
  if (!conn.connected()) {
    conn.stop();
    Serial.println(F("No connection!"));
    Serial.flush();

    connectToServer();
  }

  readData();
  tentacle.processPins();
  sendData(tentacle.getPins());
}

void readData() {
  delay(DELAY);
  while (conn.available()) {
    Serial.println(F("DATA WAS AVAILABLE!"));
    Serial.flush();

    TentacleMessage message = pseudopod.readMessage();
    processMessage(message);
  }
}

void processMessage(TentacleMessage& message) {

  if(message.getTopic() == TentacleMessage::action) {
    tentacle.processPins(message.getPins(), true);

    delay(DELAY);
    pseudopod.sendPins(message.getPins());
    Serial.println(F("Sent pins"));
    Serial.flush();
    return;
  }

  if(message.getTopic() == TentacleMessage::config) {

    tentacle.configurePins(message.getPins());
    Serial.println(F("configured pins"));
    Serial.flush();
    return;
  }

  Serial.println(F("got some topic I don't know about. Ignoring it"));
  Serial.flush();

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
    delay(1000);
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

void printToken(int identifier) {
  Serial.print(F("#"));
  Serial.print(identifier);
  Serial.print(F(" token: "));
  Serial.println(token);
}
