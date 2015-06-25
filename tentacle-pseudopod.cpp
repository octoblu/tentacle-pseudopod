#include "tentacle-pseudopod.h"
#include <stddef.h>
#include "Arduino.h"

Pseudopod::Pseudopod(Stream &input, Print &output, size_t numPins) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);
  pinBuffer = new PinBuffer(numPins);
}

PinBuffer& Pseudopod::getPinBuffer() {
  return *pinBuffer;
}

size_t Pseudopod::sendPins() {
  pbOutput.bytes_written = 0;

  protobuf::TentacleMessage protobufMsg = {};
  protobufMsg.topic = protobuf::TentacleMessageTopic_action;
  protobufMsg.has_topic = true;
  protobufMsg.response = true;
  protobufMsg.has_response = true;
  protobufMsg.pins.funcs.encode = &Pseudopod::pinEncode;
  protobufMsg.pins.arg = (void*)pinBuffer;

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

size_t Pseudopod::sendPins(PinBuffer& pins) {
  pinBuffer->reset();

  Serial.println("pins about to be sent: ");

  for(int i = 0; i < pins.size(); i++) {
    Pin& pin = pins.getPin(i);
    printPin(pin);
    pinBuffer->updatePin(pin);
  }

  sendPins();
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
  pbOutput.bytes_written = 0;

  protobuf::MeshbluAuthentication protobufMsg = {};
  bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);

  return pbOutput.bytes_written;
}

TentacleMessage::Topic Pseudopod::readMessage() {
  pinBuffer->reset();

  protobuf::TentacleMessage protobufMsg = {};

  protobufMsg.pins.funcs.decode = &Pseudopod::pinDecode;
  protobufMsg.pins.arg = (void*) &pinBuffer;

  bool status = pb_decode_delimited(&pbInput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.print(F("Number of Pins Received: "));
  Serial.println(pinBuffer->size());
  Serial.println(F("I received the following pins: "));
  for(int i = 0; i < pinBuffer->size(); i++) {
    Pin &pin = pinBuffer->getPin(i);
    printPin(pin);
  }

  if(protobufMsg.topic == protobuf::TentacleMessageTopic_action) {
    return TentacleMessage::action;
  }

  if(protobufMsg.topic == protobuf::TentacleMessageTopic_config) {
    return TentacleMessage::config;
  }

  return TentacleMessage::unknown;
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
  Serial.println("entered pinEncode");
  Serial.flush();
  PinBuffer *pinBuffer = (PinBuffer*) *arg;
  Serial.println("cast pinbuffer out");
  Serial.flush();
  bool fail = false;
  for(int i = 0; i < pinBuffer->size(); i++) {
    Pin &pin = pinBuffer->getPin(i);
    Serial.println("got pin: ");
    printPin(pin);

    protobuf::Pin protobufPin = {};

    protobufPin.number = pin.getNumber();
    protobufPin.has_number = true;

    protobufPin.value = pin.getValue();
    protobufPin.has_value = true;

    protobufPin.action = getProtoBufAction(pin.getAction());
    protobufPin.has_action = true;

    if (!pb_encode_tag_for_field(stream, field)) {
      return false;
    }

    if(!pb_encode_submessage(stream, protobuf::Pin_fields, &protobufPin)) {
      return false;
    }
  }

  return true;
}


bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  PinBuffer* pinBuffer = (PinBuffer*) *arg;

  protobuf::Pin protobufPin = {};

  if (!pb_decode(stream, protobuf::Pin_fields, &protobufPin)) {
    return false;
  }
  Serial.print("My size is: ");
  Serial.flush();
  Serial.print(pinBuffer->size());
  Serial.flush();

  printPin(protobufPin);

  pinBuffer->updatePin(protobufPin.number, getPinAction(protobufPin.action), protobufPin.value );

  printPin(pinBuffer->getPin(protobufPin.number));

  return true;
}

void Pseudopod::printPin(const Pin& pin) {
  Serial.print(F("#"));
  Serial.print(pin.getNumber());
  Serial.print(F("\taction:\t"));
  Serial.print(pin.getAction());
  Serial.print(F("\tvalue:\t"));
  Serial.println(pin.getValue());
  Serial.flush();
}

void Pseudopod::printPin(const protobuf::Pin& protobufPin) {
  Serial.print("Protobuf pin: #");
  Serial.print(protobufPin.number);
  Serial.print("\taction:\t");
  Serial.print(protobufPin.action);
  Serial.print("\tvalue:\t");
  Serial.println(protobufPin.value);
  Serial.flush();
}
