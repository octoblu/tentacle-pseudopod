#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

#include <stddef.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"

class Pseudopod {
  public:
    Pseudopod(Stream &input, Print &output);

    size_t sendPins(Pin *pins, size_t length);
    size_t processMessage(Tentacle &tentacle);

    size_t authenticate(const char* uuid, const char *token);
    size_t registerDevice();

   private:
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;

    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
    static protobuf::Action getProtoBufAction(Pin::Action action);
    static Pin::Action getPinAction(protobuf::Action action);
};
#endif
