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

Pseudopod *pseudopod;
TentacleArduino  tentacle;

char ssid[] = "octoblu-guest";
char password[] = "octoblu1";

//192.168.100.7
IPAddress server(172,16,42,44);

/*char server[] = "192.168.0.112";*/
#define port 8111

int status = WL_IDLE_STATUS;
WiFiClient conn;
std::vector<Pin> pins;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up.");
  setupWifi();
  pseudopod = new Pseudopod(tentacle, conn, conn);
  tentacle.configurePin(Pin(0, Pin::digitalRead));
}

int freeRam () 
{
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void loop() {
  if (!conn.connected()) {
    softReset();
  }
  Serial.print("Number of Pins:");
  Serial.println(tentacle.getNumberOfPins());
  Serial.println("writing");
  Serial.flush();

  int written = pseudopod->sendValue();

  Serial.print(written);
  Serial.println(" bytes written.");
  Serial.print(freeMemory());
  Serial.print(" : ");
  Serial.print(freeRam());
  Serial.println(" bytes free");
  Serial.flush();
  delay(2000);
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
