#include <pb_encode.h>
#include <pb_decode.h>
#include "tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

namespace tentacle {
  class Pseudopod {
  public:
    Pseudopod(Stream &input, Print &output);
    unsigned int writeStateMessage(const std::vector<Pin> &pins);
    const std::vector<Pin> readStateMessage();

  private:
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;

    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
  };
}
