#include <SPI.h>
#include <WiFi.h>
#include <StandardCplusplus.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include <tentacle-pseudopod.h>
#include <tentacle.h>

#include "BufferStream.hpp"

#include "Arduino.h"

Pseudopod *pseudopod;
Tentacle  tentacle;

char ssid[] = "octoblu-guest";
char password[] = "octoblu1";

//192.168.100.7
IPAddress server(172,16,42,44);

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
  std::vector<Pin> pins;
  for(int i = 0; i < 300; i++) {
    pins.push_back(Pin(i,NULL, i));
  }
  int written = pseudopod->writeStateMessage(pins);
  Serial.print(written);
  Serial.println("bytes written.");
  Serial.flush();
  delay(2000);
}

void setupWifi() {
  initWifi();

  Serial.println("\nWifi initialized. Connecting to server.");
  printWifiStatus();
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

    delay(3000);
  }
}

void printWifiStatus() {
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
}

void softReset() {
  asm volatile ("  jmp 0");
}
