#include "BufferStream.hpp"
#include <EEPROM.h>
BufferStream::BufferStream() {
  this->bufferLength = EEPROM.end();
  this->readden = 0;
  this->written = 0;
}

void setup() {

}

int BufferStream::available() {
  return written - readden;
}

int BufferStream::read() {
  if (available() < 1) {
    return -1;
  }

  return EEPROM[readden++];
}

int BufferStream::peek() {
  return EEPROM[readden];
}

void BufferStream::flush() {
}

bool BufferStream::find(char *target) {
  return false;
}

bool BufferStream::find(uint8_t *target) {
  return false;
}

bool BufferStream::find(char *target, size_t length) {
  return false;
}

bool BufferStream::find(uint8_t *target, size_t length) {
  return false;
}

size_t BufferStream::readBytes( char *buffer, size_t length) {
  return readBytes( (uint8_t *) buffer, length);
}

size_t BufferStream::readBytes( uint8_t *buffer, size_t length) {
  if (available() < 1) {
    return 0;
  }

  for(int i = 0; i < length; i++ ){
    buffer[i] = EEPROM[i+readden];
  }

  size_t newRead = readden + length;
  size_t written = length;

  if (readden + length > this->written) {
    written = this->written - readden;
    readden = this->written;
  } else {
    readden += length;
  }

  return written;
}

size_t BufferStream::write(const uint8_t *buffer, size_t length) {

  for(int i = 0; i < length; i++ ){
    EEPROM[i+written] = buffer[i];
  }

  written += length;

  return length;
}

size_t BufferStream::write(uint8_t buffer) {
  return write(&buffer, 1);
}
