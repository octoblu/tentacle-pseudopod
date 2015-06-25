#include "tentacle-pseudopod.h"
#include <stddef.h>
#include "Arduino.h"

Pseudopod::Pseudopod(Stream &input, Print &output, Tentacle& tentacle) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);

  this->tentacle = &tentacle;
  pinActions = new Action[tentacle.getNumPins()];
  resetPinActions();
}

void Pseudopod::resetPinActions() {
  for(int i = 0; i < tentacle->getNumPins(); i++) {
    pinActions[i] = Action_ignore;
  }
}

size_t Pseudopod::sendPins() {
  pbOutput.bytes_written = 0;

  currentMessage = {};
  currentMessage.topic = TentacleMessageTopic_action;
  currentMessage.has_topic = true;
  currentMessage.response = true;
  currentMessage.has_response = true;
  currentMessage.pins.funcs.encode = &Pseudopod::pinEncode;
  currentMessage.pins.arg = (void*)this;

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

size_t Pseudopod::sendPins(Action* actions) {
  resetPinActions();

  Serial.println(F("pins about to be sent: "));

  for(int i = 0; i < tentacle->getNumPins(); i++) {
    pinActions[i] = actions[i];
  }

  return sendPins();
}

size_t Pseudopod::sendConfiguredPins() {
  return sendPins(tentacle->getPinActions());
}

size_t Pseudopod::authenticate(const char *uuid, const char *token) {
  Serial.print(F("uuid in string:\t"));
  Serial.println(uuid);

  Serial.print(F("token in string:\t"));
  Serial.println(token);
  Serial.flush();

  pbOutput.bytes_written = 0;

  currentMessage = {};

  currentMessage.topic = TentacleMessageTopic_authentication;
  currentMessage.has_topic = true;
  currentMessage.authentication = {};
  currentMessage.has_authentication = true;

  strncpy(currentMessage.authentication.uuid, uuid, 36);
  currentMessage.authentication.has_uuid = true;

  strncpy(currentMessage.authentication.token, token, 40);

  currentMessage.authentication.has_token = true;

  Serial.print(F("uuid:\t"));
  Serial.println(currentMessage.authentication.uuid);

  Serial.print(F("token:\t"));
  Serial.println(currentMessage.authentication.token);

  Serial.flush();
  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  return pbOutput.bytes_written;
}

size_t Pseudopod::registerDevice() {
  pbOutput.bytes_written = 0;

  MeshbluAuthentication message = {};
  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &message);

  return pbOutput.bytes_written;
}

TentacleMessageTopic Pseudopod::readMessage() {

  Serial.println("resetting pins");
  Serial.flush();

  resetPinActions();

  Serial.println("pins reset");
  Serial.flush();

  currentMessage = {};
  currentMessage.pins.funcs.decode = &Pseudopod::pinDecode;
  currentMessage.pins.arg = (void*) this;

  bool status = pb_decode_delimited(&pbInput, TentacleMessage_fields, &currentMessage);
  return currentMessage.topic;
}

bool Pseudopod::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
  Serial.println("entered pinEncode");
  Serial.flush();

  Pseudopod *pseudopod = (Pseudopod*) *arg;
  Action action;
  Pin pin;

  for(int i = 0; i < pseudopod->tentacle->getNumPins(); i++) {
    action = pseudopod->pinActions[i];
    if(action == Action_ignore) {
      continue;
    }
    Serial.print(F("Not ignoring pin #"));
    Serial.println(i);
    Serial.flush();
    pin = {};
    pin.has_action = true;
    pin.action = action;
    pin.has_number = true;
    pin.number = i;
    int value = pseudopod->tentacle->processPin(i);

    if(value != -1) {
      pin.has_value = true;
      pin.value = value;
    }

    printPin(pin);

    if (!pb_encode_tag_for_field(stream, field)) {
      return false;
    }

    if(!pb_encode_submessage(stream, Pin_fields, &pin)) {
      return false;
    }

  }

  return true;
}


bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  Pseudopod *pseudopod = (Pseudopod*) *arg;

  Pin pin = {};

  if (!pb_decode(stream, Pin_fields, &pin)) {
    return false;
  }

  TentacleMessage& message = pseudopod->currentMessage;

  switch(message.topic) {
    case TentacleMessageTopic_config:
      Serial.println("IT WAS A CONFIG TOPIC");
      pseudopod->tentacle->configurePin(pin.number, pin.action);
    break;
    case TentacleMessageTopic_action:
      Serial.println("IT WAS AN ACTION TOPIC");
      pseudopod->pinActions[pin.number] = pin.action;
      pseudopod->tentacle->processPin(pin.number, pin.value);
    break;
  }

  printPin(pin);
  return true;
}

void Pseudopod::printPin(const Pin& pin) {
  Serial.print("pin: #");
  Serial.print(pin.number);
  Serial.print("\taction:\t");
  Serial.print(pin.action);
  Serial.print("\tvalue:\t");
  Serial.println(pin.value);
  Serial.flush();
}
