#include "tentacle-protocol-buffer.h"
namespace tentacle {
  TentacleProtoBuf::TentacleProtoBuf(uint8_t *buffer, size_t bufferLength) {
    this->buffer = buffer;
    this->bufferLength = bufferLength;
  }

  unsigned int TentacleProtoBuf::writeStateMessage(const std::vector<Pin> &pins) {

    protobuf::MicrobluState message = {};
    // message.has_pins = true;
    message.pins.funcs.encode = &TentacleProtoBuf::pinEncode;
    message.pins.arg = (void*) &pins;
    // message.pins.funcs.decode = NULL;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufferLength);
    bool status = pb_encode(&stream, protobuf::MicrobluState_fields, &message);
    unsigned int messageSize = stream.bytes_written;
    return messageSize;
  }

  const std::vector<Pin> TentacleProtoBuf::readStateMessage(unsigned int messageSize) {
    std::vector<Pin> pins;
    protobuf::MicrobluState message = {};
    // message.pins.funcs.encode = NULL;
    message.pins.funcs.decode = &TentacleProtoBuf::pinDecode;
    message.pins.arg = (void*) &pins;

    pb_istream_t stream = pb_istream_from_buffer(buffer, messageSize);
    bool status = pb_decode(&stream, protobuf::MicrobluState_fields, &message);

    return pins;
  }

  bool TentacleProtoBuf::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

    std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
    bool fail = false;
    for(Pin pin : *pins) {
      protobuf::Pin protoBufPin;

      protoBufPin.number = pin.getNumber();
      protoBufPin.value = pin.getValue();

      if (!pb_encode_tag_for_field(stream, field)) {
        return false;
      }

      if(!pb_encode_submessage(stream, protobuf::Pin_fields, &protoBufPin)) {
        return false;
      }
    }
    return true;
  }

  bool TentacleProtoBuf::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg)
  {
    std::vector<Pin> *pins = (std::vector<Pin>*) *arg;

    protobuf::Pin protoBufPin;

    if (!pb_decode(stream, protobuf::Pin_fields, &protoBufPin)) {
        return false;
    }
    Pin pin(protoBufPin.number, NULL, protoBufPin.value);

    pins->push_back(pin);

    return true;
  }
}
