#include <pb_encode.h>
#include <pb_decode.h>
#include "tentacle.hpp"
#include "proto-buf.hpp"

namespace tentacle {
  class TentacleProtoBuf {
  public:
    TentacleProtoBuf(uint8_t *buffer, size_t bufferLength);
    void writeStateMessage(const std::vector<Pin> &pins);
    const std::vector<Pin> readStateMessage();
  private:
    uint8_t *buffer;
    size_t bufferLength;
    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);    
  };
}
