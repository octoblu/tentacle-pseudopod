#include "tentacle-protocol-buffer.hpp"
namespace tentacle {
  TentacleProtoBuf::TentacleProtoBuf(uint8_t *buffer, size_t bufferLength) {
    this->buffer = buffer;
    this->bufferLength = bufferLength;
  }

  void TentacleProtoBuf::writeStateMessage(const std::vector<Pin> &pins) {
    protobuf::MicrobluState message;
    message.pins.funcs.encode = &TentacleProtoBuf::pinEncode;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferLength);
    pb_encode(&stream, protobuf::MicrobluState_fields, &message);
  }

  const std::vector<Pin> TentacleProtoBuf::readStateMessage() {
    protobuf::MicrobluState message;
    pb_istream_t stream = pb_istream_from_buffer(buffer, bufferLength);
    pb_decode(&stream, protobuf::MicrobluState_fields, &message);
  }

  bool TentacleProtoBuf::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
    std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
    bool fail = false;
    for(Pin pin : *pins) {
      protobuf::Pin protoBufPin;

      protoBufPin.number = pin.getPin();
      protoBufPin.value = pin.getState();

      if (!pb_encode_tag_for_field(stream, field)) {
        return false;
      }

      if(!pb_encode_submessage(stream, protobuf::Pin_fields, &protoBufPin)) {
        return false;
      }
    }
    return true;
  }
}
