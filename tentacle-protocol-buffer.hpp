#include <pb_encode.h>
#include <pb_decode.h>
#include "protocols/microblu.pb.h"
#include "tentacle.hpp"

namespace tentacle {
  class TentacleProtoBuf {
  public:
    TentacleProtoBuf(uint8_t *buffer, size_t bufferLength);
    void writeStateMessage(const std::vector<Pin> &pins);
  private:
    uint8_t *buffer;
    size_t bufferLength;
  };
}
