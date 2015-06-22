#include "tentacle-pseudopod.h"
#include "pin-array.h"
#include <stddef.h>
#include "Arduino.h"



Pseudopod::Pseudopod(Stream &input, Print &output, size_t numPins) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);

  this->numPins = numPins;
  pinBuffer = PinArray(numPins);
  resetPinBuffer();
}

void Pseudopod::resetPinBuffer() {
  Serial.println(F("Resetting pin buffer"));
  Serial.flush();

  pinBuffer.resetPins();

  Serial.println(F("Pin buffer reset."));
  Serial.flush();
}

size_t Pseudopod::sendPins(Pin *pins, size_t length) {
  pinBuffer.update(pins, length);

  pbOutput.bytes_written = 0;

  protobuf::TentacleMessage protobufMsg = {};
  protobufMsg.topic = protobuf::TentacleMessageTopic_action;
  protobufMsg.has_topic = true;
  protobufMsg.response = true;
  protobufMsg.has_response = true;
  protobufMsg.pins.funcs.encode = &Pseudopod::pinEncode;
  protobufMsg.pins.arg = (void*)&pinBuffer;

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode_delimited(&pbOutput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

size_t Pseudopod::sendPins(const PinArray& pinArray) {
  return sendPins(pinArray.getElements(), pinArray.getLength());
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

TentacleMessage Pseudopod::readMessage() {
  Serial.println(F("Entered readMessage"));
  Serial.flush();

  resetPinBuffer();

  protobuf::TentacleMessage protobufMsg = {};

  protobufMsg.pins.funcs.decode = &Pseudopod::pinDecode;
  protobufMsg.pins.arg = (void*) &pinBuffer;

  bool status = pb_decode_delimited(&pbInput, protobuf::TentacleMessage_fields, &protobufMsg);

  Serial.println(F("I received the following pins: "));
  for(int i = 0; i < numPins; i++) {
    Pin &pin = pinBuffer[i];

    if(pin.getAction() == Pin::ignore) {
      continue;
    }

    printPin(pin);
  }

  if(protobufMsg.topic == protobuf::TentacleMessageTopic_action) {
    return TentacleMessage(TentacleMessage::action, pinBuffer);
  }

  if(protobufMsg.topic == protobuf::TentacleMessageTopic_config) {
    return TentacleMessage(TentacleMessage::config, pinBuffer);
  }

  return TentacleMessage(TentacleMessage::unknown, pinBuffer);
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
  PinArray pinArray = *((PinArray*) *arg);
  bool fail = false;
  for(int i = 0; i < pinArray.getLength(); i++) {

    Pin &pin = pinArray[i];

    if(pin.getAction() == Pin::ignore) {
      continue;
    }

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
  PinArray pinArray = *((PinArray*) *arg);

  protobuf::Pin protobufPin = {};

  if (!pb_decode(stream, protobuf::Pin_fields, &protobufPin)) {
    return false;
  }

  Serial.print("Protobuf pin: #");
  Serial.print(protobufPin.number);
  Serial.print("\tvalue:\t");
  Serial.print(protobufPin.value);
  Serial.print("\taction:\t");
  Serial.flush();

  Pin& pin = pinArray[protobufPin.number];

  pin.setAction( getPinAction(protobufPin.action) );
  pin.setValue(protobufPin.value);

  printPin(pin);

  return true;
}

void Pseudopod::printPin(Pin &pin) {
  Serial.print(F("#"));
  Serial.print(pin.getNumber());
  Serial.print(F("\tmode:\t"));
  Serial.print(pin.getAction());
  Serial.print(F("\tvalue:\t"));
  Serial.println(pin.getValue());
  Serial.flush();
}
