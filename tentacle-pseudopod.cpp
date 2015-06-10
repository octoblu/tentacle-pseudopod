#include "tentacle-pseudopod.h"
#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"
#include "Arduino.h"

//TODO: Fix THIS broken code! Should be somewhere else....
Pseudopod::Pseudopod(Tentacle &tentacle, Stream &input, Print &output) {
  pb_istream_from_stream(input, pbInput);
  pb_ostream_from_stream(output, pbOutput);
  this->tentacle = &tentacle;
}

unsigned int Pseudopod::sendValue(Pin *pins, int length) {
  pbOutput.bytes_written = 0;
  protobuf::TentacleMessage message = {};

  message.topic = protobuf::Topic_action;
  message.has_topic = true;
  message.response = true;
  message.has_response = true;
  message.pins.funcs.encode = &Pseudopod::pinEncodeValue;

  std::vector<Pin> values = tentacle->getValue(pins, length);
  message.pins.arg = (void*)&values;
  bool status = pb_encode(&pbOutput, protobuf::TentacleMessage_fields, &message);
  (pbOutput.callback)(&pbOutput,{0x0},1);
  unsigned int messageSize = pbOutput.bytes_written;
  return messageSize;
}

unsigned int Pseudopod::sendValue() {
  return sendValue(tentacle->getConfig(), tentacle->getNumPins());
}

 bool Pseudopod::readMessage() {
  std::vector<Pin> pins;
  protobuf::TentacleMessage message = {};

  message.pins.funcs.decode = &Pseudopod::pinDecode;
  message.pins.arg = (void*) &pins;

  bool status = pb_decode(&pbInput, protobuf::TentacleMessage_fields, &message);
  switch(message.topic) {

    case protobuf::Topic_action:    
      sendValue(&pins[0], pins.size());
      Serial.println(F("Got an ACTION topic!"));
    break;

    case protobuf::Topic_config:
      tentacle->configurePins(pins);
      Serial.println(F("Got an CONFIG topic!"));
    break;
  }
  return status;
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
