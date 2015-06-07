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

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");

  WiFiClient conn = setupWifi();
  Serial.println("wifi should be setup!");

  pseudopod = new Pseudopod(conn, conn);

  pins.push_back(Pin(4, 0, 1));
  pins.push_back(Pin(40, 0, 0));

}

void loop() {
  pseudopod->writeStateMessage(pins);  
}

WiFiClient setupWifi() {
  WiFiClient client = initWifi();

  Serial.println("\nWifi initialized. Connecting to server.");
  while(!client.connect(server, port)) {
    Serial.println("Can't connect to the server. Rebooting.");
    Serial.flush();
    softReset();
  }

  Serial.println("connected to server");
  Serial.flush();

  return client;
}

WiFiClient initWifi() {
  WiFiClient client;
  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    Serial.flush();

    status = WiFi.begin(ssid, password);

    delay(5000);
  }

  return client;
}

void softReset() {
  asm volatile ("  jmp 0");
}
