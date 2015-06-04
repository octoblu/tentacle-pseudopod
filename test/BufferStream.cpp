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
  cout << "BufferStream::available() " << written - readden << endl << endl;
  return written - readden;
}

int BufferStream::read() {
  cout << "BufferStream::read() " << readden << endl << endl;
  return buffer[readden++];

}

int BufferStream::peek() {
  cout << "BufferStream::peek() " << readden << endl << endl;
  return buffer[readden];
}

void BufferStream::flush() {
  cout << "BufferStream::flush() " << endl << endl;
}


bool BufferStream::find(char *target) {
  cout << "BufferStream::find() " << endl << endl;
  return false;
}

bool BufferStream::find(uint8_t *target) {
  cout << "BufferStream::find() " << endl << endl;
  return false;
}


bool BufferStream::find(char *target, size_t length) {
  cout << "BufferStream::find() " << endl << endl;
  return false;
}

bool BufferStream::find(uint8_t *target, size_t length) {
  cout << "BufferStream::find() " << endl << endl;
  return false;
}

size_t BufferStream::readBytes( char *buffer, size_t length) {
  cout << "BufferStream::readBytes() " << buffer << ' '  << (unsigned int)this->buffer[readden] << ' '<< length << endl << endl;
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

  return readBytes( (char*) buffer, length);
}

size_t BufferStream::write(const uint8_t *buffer, size_t length) {

  cout << "BufferStream::write() ";
  for(int i = 0; i < length; i++) {
    cout << (short)buffer[i] << ' ';
  }
  cout << endl;

  memcpy(&this->buffer[written], buffer, length);
  cout << "BufferStream::write2() ";

  for(int i = 0; i < length; i++) {
    cout << (short)this->buffer[written + i] << ' ';
  }

  cout << endl << endl;

  written += length;
  return length;

}
