extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

#include "tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"

class Pseudopod {
public:
  Pseudopod(Tentacle &tentacle, Stream &input, Print &output);
  unsigned int sendValue();
  void readMessage();

private:
  pb_ostream_t pbOutput;
  pb_istream_t pbInput;

  Tentacle *tentacle;

  static bool pinEncodeConfig(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
  static bool pinEncodeValue(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
  static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
  static protobuf::Action getProtoBufAction(Pin::Action action);
  static Pin::Action getPinAction(protobuf::Action action);
};
