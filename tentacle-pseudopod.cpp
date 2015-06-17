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

size_t Pseudopod::processMessage(const Tentacle &tentacle) {
  Pin pins[tentacle.getNumPins()];

  protobuf::TentacleMessage protobufMsg = {};

  protobufMsg.pins.funcs.decode = &Pseudopod::pinDecode;
  protobufMsg.pins.arg = (void*) &pins;

  bool status = pb_decode_delimited(&pbInput, protobuf::TentacleMessage_fields, &protobufMsg);
  switch(protobufMsg.topic) {

    case protobuf::TentacleMessageTopic_action:
      Serial.println(F("Got an ACTION topic!"));

    break;

    case protobuf::TentacleMessageTopic_config:
      Serial.println(F("Got an CONFIG topic!"));

    break;

  }

  return 0;
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

bool Pseudopod::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {

  Tentacle *tentacle = (Tentacle*) *arg;
  bool fail = false;
  for(int i = 0; i < tentacle->getNumPins(); i++) {

    Pin pin = tentacle->getValue(i);

    protobuf::Pin protoBufPin;

    protoBufPin.number = pin.getNumber();
    protoBufPin.has_number = true;

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


bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  Pin *pins = (Pin*) *arg;
  protobuf::Pin protoBufPin = {};

  if (!pb_decode(stream, protobuf::Pin_fields, &protoBufPin)) {
    return false;
  }

  Pin pin((int)protoBufPin.number, getPinAction(protoBufPin.action), protoBufPin.value);

  pins[pin.getNumber()] = pin;

  return true;
}
