#include "Stream.h"

#ifndef BUFFER_STREAM_H
#define BUFFER_STREAM_H

class BufferStream : public Stream {
  public:
    BufferStream(uint8_t *buffer, int bufferLength);
    int available();
    int read();
    int peek();
    void flush();

    bool find(char *target);
    bool find(uint8_t *target);

    bool find(char *target, size_t length);
    bool find(uint8_t *target, size_t length);

    size_t readBytes( char *buffer, size_t length);
    size_t readBytes( uint8_t *buffer, size_t length);

    size_t write(const uint8_t *buffer, size_t size);
  private:
    uint8_t *buffer;
    int readden; //I know. Read is a verb and a noun. deal with it.
    int written;
    int bufferLength;
};

#endif
