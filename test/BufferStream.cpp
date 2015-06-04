#include "BufferStream.hpp"
#include <stdio.h>
using namespace std;
BufferStream::BufferStream(uint8_t *buffer, int bufferLength) {
  this->buffer = buffer;
  this->bufferLength = bufferLength;
  this->readden = 0;
  this->written = 0;
}

int BufferStream::available() {
  cout << "BufferStream::available() " << written - readden << endl;
  return written - readden;
}

int BufferStream::read() {
  cout << "BufferStream::read() " << readden << endl;
  return buffer[readden++];

}

int BufferStream::peek() {
  cout << "BufferStream::peek() " << readden << endl;
  return buffer[readden];
}

void BufferStream::flush() {
  cout << "BufferStream::flush() " << endl;
}


bool BufferStream::find(char *target) {
  cout << "BufferStream::find() " << endl;
  return false;
}

bool BufferStream::find(uint8_t *target) {
  cout << "BufferStream::find() " << endl;
  return false;
}


bool BufferStream::find(char *target, size_t length) {
  cout << "BufferStream::find() " << endl;
  return false;
}

bool BufferStream::find(uint8_t *target, size_t length) {
  cout << "BufferStream::find() " << endl;
  return false;
}

size_t BufferStream::readBytes( char *buffer, size_t length) {
  cout << "BufferStream::readBytes() " << buffer << ' ' << length << endl;
  memcpy(&buffer, &this->buffer[readden], length);

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

size_t BufferStream::readBytes( uint8_t *buffer, size_t length) {
  cout << "BufferStream::readBytes() " << buffer << ' ' << length << endl;
  buffer = &this->buffer[readden];

  size_t newRead = readden + length;
  size_t written = length;

  if (readden + length > this->written) {
    written = this->written - readden;
    readden = this->written;
  }

  return written;
}

size_t BufferStream::write(const uint8_t *buffer, size_t length) {
  cout << "BufferStream::write() " << (unsigned int)*buffer << ' ' << length << endl;
  memcpy(&this->buffer[readden], &buffer, length);
  written += length;
}
