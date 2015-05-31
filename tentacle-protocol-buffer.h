#include <pb_encode.h>
#include <pb_decode.h>
#include <iostream>
#include "tentacle.h"
#include "proto-buf.hpp"

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

namespace tentacle {
  class TentacleProtoBuf {
  public:
    TentacleProtoBuf(Stream input, Stream output);
    unsigned int writeStateMessage(const std::vector<Pin> &pins);
    const std::vector<Pin> readStateMessage();

  private:
    Stream input;
    Stream output;
    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
  };
}
