#include "tentacle-pseudopod.h"
#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"
namespace tentacle {
  Pseudopod::Pseudopod(Stream &input, Print &output) {
    pb_istream_from_stream(input, pbInput);
    pb_ostream_from_stream(output, pbOutput);
  }

  unsigned int Pseudopod::writeStateMessage(const std::vector<Pin> &pins) {
    protobuf::MicrobluState message = {};
    message.pins.funcs.encode = &Pseudopod::pinEncode;
    message.pins.arg = (void*) &pins;

    bool status = pb_encode(&pbOutput, protobuf::MicrobluState_fields, &message);
    unsigned int messageSize = pbOutput.bytes_written;
    return messageSize;
  }

  const std::vector<Pin> Pseudopod::readStateMessage() {
    std::vector<Pin> pins;
    protobuf::MicrobluState message = {};

    message.pins.funcs.decode = &Pseudopod::pinDecode;
    message.pins.arg = (void*) &pins;
    bool status = pb_decode(&pbInput, protobuf::MicrobluState_fields, &message);

    return pins;
  }

  bool Pseudopod::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

    std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
    bool fail = false;
    for(int i = 0; i < pins->size(); i++) {
      Pin pin = pins->at(i);
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

  bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg)
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
