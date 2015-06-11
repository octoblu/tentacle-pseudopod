#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

#include <vector>
#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"
#include "Arduino.h"

#include "tentacle-message.h"
#include "Stream.h"
#include "proto-buf.hpp"

class Pseudopod {
public:
  static size_t sendPins(const std::vector<Pin> &pins, Print &output);
  static TentacleMessage getMessage(Stream &input);

private:
  // pb_ostream_t pbOutput;
  // pb_istream_t pbInput;

  static bool pinEncodeConfig(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
  static bool pinEncodeValue(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
  static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
  static protobuf::Action getProtoBufAction(Pin::Action action);
  static Pin::Action getPinAction(protobuf::Action action);
};
#endif
