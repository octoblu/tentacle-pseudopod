#include <SPI.h>
#include <WiFi.h>
#include <StandardCplusplus.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "MemoryFree.h"

#include "tentacle.h"
#include "tentacle-arduino.h"
#include "tentacle-pseudopod.h"

#include "BufferStream.hpp"
#include "Arduino.h"
#include <string>
#include <EEPROM.h>

TentacleArduino  tentacle;


char ssid[] = "octoblu-guest";
char password[] = "octoblu1";

//192.168.100.7
IPAddress server(172,16,42,44);

/*char server[] = "192.168.0.112";*/
#define port 8111

Pseudopod *pseudopod;
Pseudopod *pseudopod2;

int status = WL_IDLE_STATUS;
WiFiClient conn;
std::vector<Pin> pins;
void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");
  setupWifi();
  BufferStream b = BufferStream();
  pseudopod = new Pseudopod(conn, conn);
  pseudopod2 = new Pseudopod(conn, b);

}

int freeRam ()
{
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

void loop() {
  if (!conn.connected()) {
    Serial.print("I wasn't connected!");
    Serial.flush();
    softReset();
  }
  Serial.print("Number of Pins:");
  Serial.println(tentacle.getNumPins());
  Serial.println("writing");
  Serial.flush();
  sendData();
  readData();
}

void sendData() {
  //tentacle.getConfig()[2].action = Pin::digitalRead;
  int written = pseudopod->sendValue(
    tentacle.getValue()
  );

  Serial.print(written);
  Serial.println(" bytes written.");
  Serial.print(freeMemory());
  Serial.print(" : ");
  Serial.print(freeRam());
  Serial.println(" bytes free");
  Serial.flush();
}

void readData() {
  while (conn.available()) {
    delay(2000);
    Serial.println("DATA WAS AVAILABLE!");
    Serial.flush();

    std::vector<Pin> pins = pseudopod->readMessage();
    tentacle.configurePins(pins);

    Serial.print("Was the topic successful?: ");
    Serial.println(pins.size());
  }
}

void connectToServer() {
  Serial.println("Connecting to the server.");
  Serial.flush();
  while(!conn.connect(server, port)) {
    Serial.println("Can't connect to the server.");
    Serial.flush();
    conn.stop();
    delay(1000);
  }
}

void setupWifi() {
  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.flush();
    status = WiFi.begin(ssid, password);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.flush();

  connectToServer();
}

void softReset() {
  asm volatile ("  jmp 0");
}
