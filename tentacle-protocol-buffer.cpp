#include "tentacle-protocol-buffer.hpp"
namespace tentacle {
  TentacleProtoBuf::TentacleProtoBuf(uint8_t *buffer, size_t bufferLength) {
    this->buffer = buffer;
    this->bufferLength = bufferLength;
  }

  void TentacleProtoBuf::writeStateMessage(const std::vector<Pin> &pins) {
    MicrobluState message;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferLength);
    pb_encode(&stream, MicrobluState_fields, &message);
  }
}
