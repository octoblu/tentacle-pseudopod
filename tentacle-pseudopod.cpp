#include "tentacle-pseudopod.h"
#include "Arduino.h"
//#include "//Serial.h"
Pseudopod::Pseudopod(Stream &input, Print &output) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);
}

size_t Pseudopod::sendPins(const Tentacle &tentacle) {
  pbOutput.bytes_written = 0;

  protobuf::TentacleMessage protobufMsg = {};
  protobufMsg.topic = protobuf::TentacleMessageTopic_action;
  protobufMsg.has_topic = true;
  protobufMsg.response = true;
  protobufMsg.has_response = true;
  protobufMsg.pins.funcs.encode = &Pseudopod::pinEncode;
  protobufMsg.pins.arg = (void*)&tentacle;

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

size_t Pseudopod::authenticate(const char *uuid, const char *token) {
  Serial.print(F("uuid in string:\t"));
  Serial.println(uuid);

  Serial.print(F("token in string:\t"));
  Serial.println(token);
  Serial.flush();

  pbOutput.bytes_written = 0;

  protobuf::TentacleMessage protobufMsg = {};
  protobufMsg.topic = protobuf::TentacleMessageTopic_authentication;
  protobufMsg.has_topic = true;
  protobufMsg.authentication = {};
  protobufMsg.has_authentication = true;

  strncpy(protobufMsg.authentication.uuid, uuid, 36);
  protobufMsg.authentication.has_uuid = true;

  strncpy(protobufMsg.authentication.token, token, 40);
  protobufMsg.authentication.has_token = true;

  Serial.print(F("uuid:\t"));
  Serial.println(protobufMsg.authentication.uuid);

  Serial.print(F("token:\t"));
  Serial.println(protobufMsg.authentication.token);

  Serial.flush();
  bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);

  return pbOutput.bytes_written;
}

size_t Pseudopod::registerDevice() {
  // pbOutput.bytes_written = 0;
  //
  // protobuf::MeshbluAuthentication protobufMsg = {};
  // bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);
  //
  // return pbOutput.bytes_written;
}

size_t Pseudopod::processMessage(Tentacle &tentacle) {
  tentacle.resetPins();

  protobuf::TentacleMessage protobufMsg = {};

  PinList pinList = { tentacle.getNumPins() };

  protobufMsg.pins.funcs.decode = &Pseudopod::pinDecode;
  protobufMsg.pins.arg = (void*) &pinList;

  bool status = pb_decode_delimited(&pbInput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.print(F("PinList size is "));
  Serial.print(pinList.size);
  Serial.flush();

  if (!status) {
    Serial.println(F("DECODE FAILED!"));
    Serial.flush();
    pinList.clear();
    return 0;
  }

  switch(protobufMsg.topic) {

    case protobuf::TentacleMessageTopic_action:
      Serial.println(F("Got an ACTION topic!"));

    break;

    case protobuf::TentacleMessageTopic_config:
      Serial.println(F("Got an CONFIG topic!"));
    break;

  }

  Serial.println(F("Printing what I think my pins are: "));
  for(int i = 0; i < tentacle.getNumPins(); i++) {
    Pin &pin = tentacle.getPin(i);
    Serial.print("#");
    Serial.print(pin.number);
    Serial.print(" mode: ");
    Serial.print(pin.action);
    Serial.print(" value: ");
    Serial.println(pin.value);
    Serial.flush();
  }

  pinList.clear();

  return 0;
}

Pin::Action::ActionEnum Pseudopod::getPinAction(protobuf::Action action) {
  switch(action) {
    case protobuf::Action_digitalRead :
      return Pin::Action::digitalRead;

    case protobuf::Action_digitalWrite:
      return Pin::Action::digitalWrite;

    case protobuf::Action_analogRead  :
      return Pin::Action::analogRead;

    case protobuf::Action_analogWrite :
      return Pin::Action::analogWrite;

    case protobuf::Action_servoWrite  :
      return Pin::Action::servoWrite;

    case protobuf::Action_pwmWrite    :
      return Pin::Action::pwmWrite;

    case protobuf::Action_i2cWrite    :
      return Pin::Action::i2cWrite;

    case protobuf::Action_i2cRead     :
      return Pin::Action::i2cRead;

    case protobuf::Action_ignore      :
    default:
      return Pin::Action::ignore;
    break;
  }
}

protobuf::Action Pseudopod::getProtoBufAction(Pin::Action::ActionEnum action) {
  switch(action) {
    case Pin::Action::digitalRead :
      return protobuf::Action_digitalRead;

    case Pin::Action::digitalWrite:
      return protobuf::Action_digitalWrite;

    case Pin::Action::analogRead  :
      return protobuf::Action_analogRead;

    case Pin::Action::analogWrite :
      return protobuf::Action_analogWrite;

    case Pin::Action::servoWrite  :
      return protobuf::Action_servoWrite;

    case Pin::Action::pwmWrite    :
      return protobuf::Action_pwmWrite;

    case Pin::Action::i2cWrite    :
      return protobuf::Action_i2cWrite;

    case Pin::Action::i2cRead     :
      return protobuf::Action_i2cRead;

    case Pin::Action::ignore      :
    default:
      return protobuf::Action_ignore;
    break;
  }
}

bool Pseudopod::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  Tentacle *tentacle = (Tentacle*) *arg;
  bool fail = false;
  for(int i = 0; i < tentacle->getNumPins(); i++) {

    Pin &pin = tentacle->getPin(i);

    if (pin.action == Pin::Action::ignore) {
      continue;
    }

    protobuf::Pin protoBufPin;

    protoBufPin.number = pin.number;
    protoBufPin.has_number = true;

    protoBufPin.value = pin.value;
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


bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  PinList *pinList = (PinList*) *arg;

  protobuf::Pin protoBufPin = {};

  if (!pb_decode(stream, protobuf::Pin_fields, &protoBufPin)) {
    return false;
  }

  Pin pin = {};
  //{(int)protoBufPin.number,getPinAction(protoBufPin.action), protoBufPin.value};

  return pinList->insertPin(pin);
}
