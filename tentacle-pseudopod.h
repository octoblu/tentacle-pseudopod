#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

#include "pin-buffer.h"
#include <stddef.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"


#include "tentacle-message.h"

class Pseudopod {
  public:
    Pseudopod(Stream &input, Print &output, size_t numPins);

    size_t sendPins(PinBuffer& pins);
    size_t sendPins();

    TentacleMessage::Topic readMessage();

    size_t authenticate(const char* uuid, const char *token);
    size_t registerDevice();

    PinBuffer& getPinBuffer();

   private:
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;
    PinBuffer* pinBuffer;

    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);

    static protobuf::Action getProtoBufAction(Pin::Action action);
    static Pin::Action getPinAction(protobuf::Action action);

    static void printPin(const Pin& pin);
    static void printPin(const protobuf::Pin& protobufPin);
};

#endif
