#include <SPI.h>
#include <WiFi.h>
#include <StandardCplusplus.h>
#include <tentacle.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include <tentacle-pseudopod.h>
#include "BufferStream.hpp"

#include "Arduino.h"
using namespace tentacle;

Pseudopod *pseudopod;

std::vector<Pin> pins;

char ssid[] = "ROBOT-WASTELAND";
char password[] = "lemonade";
IPAddress server(192,168,0,112);
/*char server[] = "192.168.0.112";*/
#define port 8111

int status = WL_IDLE_STATUS;
WiFiClient conn;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");

  setupWifi();
  Serial.println("wifi should be setup!");

  pseudopod = new Pseudopod(conn, conn);
}

void loop() {
  Serial.println("writing");
  Serial.flush();
  int written = pseudopod->writeStateMessage(pins);
  Serial.print(written);
  Serial.println("bytes written.");
  Serial.flush();
  delay(2000);
}

void setupWifi() {
  initWifi();

  Serial.println("\nWifi initialized. Connecting to server.");
  while(!conn.connect(server, port)) {
    Serial.println("Can't connect to the server. Rebooting.");
    Serial.flush();
    softReset();
  }

  Serial.println("connected to server");
  Serial.flush();
}

void initWifi() {
  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.flush();

    status = WiFi.begin(ssid, password);

    delay(10000);
  }
}

void softReset() {
  asm volatile ("  jmp 0");
}
