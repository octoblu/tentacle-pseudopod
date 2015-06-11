#include "tentacle-pseudopod.h"
#include "Arduino.h"

Pseudopod::Pseudopod(Stream &input, Print &output) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);
}

size_t Pseudopod::sendPins(const std::vector<Pin> &pins) {
  pbOutput.bytes_written = 0;

  protobuf::TentacleMessage protobufMsg = {};
  protobufMsg.topic = protobuf::Topic_action;
  protobufMsg.has_topic = true;
  protobufMsg.response = true;
  protobufMsg.has_response = true;
  protobufMsg.pins.funcs.encode = &Pseudopod::pinEncodeValue;
  protobufMsg.pins.arg = (void*)&pins;

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);
  (pbOutput.callback)(&pbOutput,{0x0},1);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

TentacleMessage Pseudopod::getMessage() {
  std::vector<Pin> pins;

  protobuf::TentacleMessage protobufMsg = {};

  protobufMsg.pins.funcs.decode = &Pseudopod::pinDecode;
  protobufMsg.pins.arg = (void*) &pins;

  bool status = pb_decode(&pbInput, protobuf::TentacleMessage_fields, &protobufMsg);
  switch(protobufMsg.topic) {

    case protobuf::Topic_action:
      Serial.println(F("Got an ACTION topic!"));
      return TentacleMessage(TentacleMessage::action, pins);

    case protobuf::Topic_config:
      Serial.println(F("Got an CONFIG topic!"));
      return TentacleMessage(TentacleMessage::config, pins);
  }
}

Pin::Action Pseudopod::getPinAction(protobuf::Action action) {
  switch(action) {
    case protobuf::Action_digitalRead :
      return Pin::digitalRead;

    case protobuf::Action_digitalWrite:
      return Pin::digitalWrite;

    case protobuf::Action_analogRead  :
      return Pin::analogRead;

    case protobuf::Action_analogWrite :
      return Pin::analogWrite;

    case protobuf::Action_servoWrite  :
      return Pin::servoWrite;

    case protobuf::Action_pwmWrite    :
      return Pin::pwmWrite;

    case protobuf::Action_i2cWrite    :
      return Pin::i2cWrite;

    case protobuf::Action_i2cRead     :
      return Pin::i2cRead;

    case protobuf::Action_ignore      :
    default:
      return Pin::ignore;
    break;
  }
}

protobuf::Action Pseudopod::getProtoBufAction(Pin::Action action) {
  switch(action) {
    case Pin::digitalRead :
      return protobuf::Action_digitalRead;

    case Pin::digitalWrite:
      return protobuf::Action_digitalWrite;

    case Pin::analogRead  :
      return protobuf::Action_analogRead;

    case Pin::analogWrite :
      return protobuf::Action_analogWrite;

    case Pin::servoWrite  :
      return protobuf::Action_servoWrite;

    case Pin::pwmWrite    :
      return protobuf::Action_pwmWrite;

    case Pin::i2cWrite    :
      return protobuf::Action_i2cWrite;

    case Pin::i2cRead     :
      return protobuf::Action_i2cRead;

    case Pin::ignore      :
    default:
      return protobuf::Action_ignore;
    break;
  }
}

bool Pseudopod::pinEncodeValue(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
  bool fail = false;
  for(int i = 0; i < pins->size(); i++) {
    Pin pin = pins->at(i);
    protobuf::Pin protoBufPin;

    protoBufPin.number = pin.getNumber();
    protoBufPin.value = pin.getValue();
    protoBufPin.has_value = true;
    if (!pb_encode_tag_for_field(stream, field)) {
      return false;
    }

    if(!pb_encode_submessage(stream, protobuf::Pin_fields, &protoBufPin)) {
      return false;
    }
  }
  return true;
}

bool Pseudopod::pinEncodeConfig(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  std::vector<Pin> *pins = (std::vector<Pin>*) *arg;
  bool fail = false;
  for(int i = 0; i < pins->size(); i++) {
    Pin pin = pins->at(i);
    protobuf::Pin protoBufPin;

    protoBufPin.number = pin.getNumber();
    protoBufPin.action = getProtoBufAction(pin.getAction());
    protoBufPin.has_action = true;
    protoBufPin.pullup = pin.getPullup();
    protoBufPin.has_pullup = true;

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

  Pin pin((int)protoBufPin.number, getPinAction(protoBufPin.action), protoBufPin.value);

  pins->push_back(pin);
  return true;
}
