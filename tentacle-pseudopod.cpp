#include "tentacle-pseudopod.h"
#include <stddef.h>

Pseudopod::Pseudopod(Stream &input, Print &output, Tentacle& tentacle) {
  broadcastPins = false;
  configured = false;
  broadcastInterval = BROADCAST_INTERVAL_DEFAULT;

  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);

  this->tentacle = &tentacle;
  messagePinActions = new Action[tentacle.getNumPins()];
  resetPinActions();
}

size_t Pseudopod::authenticate(const char *uuid, const char *token) {
  pbOutput.bytes_written = 0;

  memset(&currentMessage, 0, sizeof(currentMessage));
  currentMessage.topic = TentacleMessageTopic_authentication;
  currentMessage.has_topic = true;
  currentMessage.has_authentication = true;

  strncpy(currentMessage.authentication.uuid, uuid, 36);
  currentMessage.authentication.has_uuid = true;

  strncpy(currentMessage.authentication.token, token, 40);

  currentMessage.authentication.has_token = true;

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  return pbOutput.bytes_written;
}

int Pseudopod::getBroadcastInterval() {
  return broadcastInterval;
}

bool Pseudopod::isConfigured() {
  return configured;
}

bool Pseudopod::isConnected() {
  pbOutput.bytes_written = 0;

  memset(&currentMessage, 0, sizeof(currentMessage));
  currentMessage.topic = TentacleMessageTopic_ping;
  currentMessage.has_topic = true;

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  return pbOutput.bytes_written != 0;
}

TentacleMessageTopic Pseudopod::readMessage() {
  resetPinActions();

  memset(&currentMessage, 0, sizeof(currentMessage));
  currentMessage.pins.funcs.decode = &Pseudopod::pinDecode;
  currentMessage.pins.arg = (void*) this;

  bool status = pb_decode_delimited(&pbInput, TentacleMessage_fields, &currentMessage);

  if (currentMessage.topic == TentacleMessageTopic_config) {
    for(int i = 0; i < tentacle->getNumPins(); i++) {
        tentacle->configurePin(i, toTentacleAction(messagePinActions[i]));
    }

    configured = true;
    broadcastPins = currentMessage.broadcastPins;
    broadcastInterval = currentMessage.broadcastInterval;
  }

  return currentMessage.topic;
}

size_t Pseudopod::registerDevice() {
  return 0;
}

size_t Pseudopod::requestConfiguration() {
  pbOutput.bytes_written = 0;

  memset(&currentMessage, 0, sizeof(currentMessage));
  currentMessage.topic = TentacleMessageTopic_config;
  currentMessage.has_topic = true;

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  return pbOutput.bytes_written;
}

bool Pseudopod::shouldBroadcastPins() {
  return broadcastPins;
}

size_t Pseudopod::sendConfiguredPins() {
  return sendPins(tentacle->getConfiguredPinActions());
}

size_t Pseudopod::sendPins() {
  pbOutput.bytes_written = 0;

  memset(&currentMessage, 0, sizeof(currentMessage));
  currentMessage.topic = TentacleMessageTopic_action;
  currentMessage.has_topic = true;
  currentMessage.response = true;
  currentMessage.has_response = true;
  currentMessage.pins.funcs.encode = &Pseudopod::pinEncode;
  currentMessage.pins.arg = (void*)this;

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &currentMessage);

  return pbOutput.bytes_written;
}

size_t Pseudopod::sendPins(Tentacle::Action* tentacleActions) {
  resetPinActions();

  for(int i = 0; i < tentacle->getNumPins(); i++) {
    messagePinActions[i] = fromTentacleAction(tentacleActions[i]);
  }

  return sendPins();
}

// Private

void Pseudopod::resetPinActions() {
  for(int i = 0; i < tentacle->getNumPins(); i++) {
    messagePinActions[i] = Action_ignore;
  }
}

bool Pseudopod::customDataDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  // uint64_t charInt;
  //
  // if(!pb_decode_varint(stream, &charInt)){
  //   return false;
  // }

  // char character = (char) charInt;

  // Serial.println("customDataDecode: " + character);

  return true;
}

Action Pseudopod::fromTentacleAction(Tentacle::Action tentacleAction) {
  switch (tentacleAction) {
    case Tentacle::Action_analogRead:
      return Action_analogRead;
    case Tentacle::Action_analogReadPullup:
      return Action_analogReadPullup;
    case Tentacle::Action_analogWrite:
      return Action_analogWrite;
    case Tentacle::Action_digitalRead:
      return Action_digitalRead;
    case Tentacle::Action_digitalReadPullup:
      return Action_digitalReadPullup;
    case Tentacle::Action_digitalWrite:
      return Action_digitalWrite;
    case Tentacle::Action_ignore:
      return Action_ignore;
    case Tentacle::Action_pwmWrite:
      return Action_pwmWrite;
    case Tentacle::Action_servoWrite:
      return Action_servoWrite;
    default:
      return Action_ignore;
  }
}

bool Pseudopod::pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg) {
  Pseudopod *pseudopod = (Pseudopod*) *arg;
  Pin pin;

  if (!pb_decode(stream, Pin_fields, &pin)) {
    return false;
  }

  TentacleMessage& message = pseudopod->currentMessage;
  pseudopod->messagePinActions[pin.number] = pin.action;

  if(message.topic == TentacleMessageTopic_action) {
    pseudopod->tentacle->processPin(pin.number, pin.value);
  }

  return true;
}

bool Pseudopod::pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg) {
  Pseudopod *pseudopod = (Pseudopod*) *arg;
  Action action;
  Pin pin;

  for(int i = 0; i < pseudopod->tentacle->getNumPins(); i++) {
    action = pseudopod->messagePinActions[i];
    if(action == Action_ignore) {
      continue;
    }

    pin.has_action = true;
    pin.action = action;
    pin.has_number = true;
    pin.number = i;
    int value = pseudopod->tentacle->processPin(i);

    if(value != -1) {
      pin.has_value = true;
      pin.value = value;
    }

    if (!pb_encode_tag_for_field(stream, field)) {
      return false;
    }

    if(!pb_encode_submessage(stream, Pin_fields, &pin)) {
      return false;
    }
  }

  return true;
}

Tentacle::Action Pseudopod::toTentacleAction(Action action) {
  switch (action) {
    case Action_analogRead:
      return Tentacle::Action_analogRead;
    case Action_analogReadPullup:
      return Tentacle::Action_analogReadPullup;
    case Action_analogWrite:
      return Tentacle::Action_analogWrite;
    case Action_digitalRead:
      return Tentacle::Action_digitalRead;
    case Action_digitalReadPullup:
      return Tentacle::Action_digitalReadPullup;
    case Action_digitalWrite:
      return Tentacle::Action_digitalWrite;
    case Action_ignore:
      return Tentacle::Action_ignore;
    case Action_pwmWrite:
      return Tentacle::Action_pwmWrite;
    case Action_servoWrite:
      return Tentacle::Action_servoWrite;
    default:
      return Tentacle::Action_ignore;
  }
}
