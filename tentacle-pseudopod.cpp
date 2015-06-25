#include "tentacle-pseudopod.h"
#include <stddef.h>
#include "Arduino.h"

Pseudopod::Pseudopod(Stream &input, Print &output, Tentacle& tentacle) {
  pb_ostream_from_stream(output, pbOutput);
  pb_istream_from_stream(input, pbInput);

  this->tentacle = tentacle;
  pinActions = new Action[tentacle.getNumPins()];
  resetPinActions();
}

void Pseudopod::resetPinActions() {
  for(int i = 0; i < tentacle.getNumPins(); i++) {
    pinActions[i] = Action_ignore;
  }
}

size_t Pseudopod::sendPins() {
  pbOutput.bytes_written = 0;

  TentacleMessage message = {};
  message.topic = TentacleMessageTopic_action;
  message.has_topic = true;
  message.response = true;
  message.has_response = true;
  message.has_pins = true;

  int pinCount = 0;
  for(int i = 0; i < tentacle.getNumPins(); i++) {
    Pin pin = {};

    if( pinActions[i] == Action_ignore) {
      continue;
    }

    pin.number = i;
    pin.has_number = true;

    pin.value = tentacle.getValue(i);
    pin.has_value = true;

    pin.action = pinActions[i];

    pin.has_action = true;

    message.pins[pinCount++] = pin;

  }

  Serial.println("about to encode message");
  Serial.flush();

  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &message);

  Serial.println("encoded message");
  Serial.flush();

  return pbOutput.bytes_written;
}

size_t Pseudopod::sendPins(Action* actions) {
  resetPinActions();

  Serial.println("pins about to be sent: ");

  for(int i = 0; i < tentacle.getNumPins(); i++) {
    pinActions[i] = actions[i];
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

  TentacleMessage message = {};

  message.topic = TentacleMessageTopic_authentication;
  message.has_topic = true;
  message.authentication = {};
  message.has_authentication = true;

  strncpy(message.authentication.uuid, uuid, 36);
  message.authentication.has_uuid = true;

  strncpy(message.authentication.token, token, 40);

  message.authentication.has_token = true;

  Serial.print(F("uuid:\t"));
  Serial.println(message.authentication.uuid);

  Serial.print(F("token:\t"));
  Serial.println(message.authentication.token);

  Serial.flush();
  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &message);

  return pbOutput.bytes_written;
}

size_t Pseudopod::registerDevice() {
  pbOutput.bytes_written = 0;

  MeshbluAuthentication message = {};
  bool status = pb_encode_delimited(&pbOutput, TentacleMessage_fields, &message);

  return pbOutput.bytes_written;
}

Pseudopod& Pseudopod::readMessage() {

  Serial.println("resetting pins");
  Serial.flush();

  resetPinActions();

  Serial.println("pins reset");
  Serial.flush();

  TentacleMessage message = {};

  bool status = pb_decode_delimited(&pbInput, TentacleMessage_fields, &message);

  Serial.print(F("Number of Pins Received: "));
  Serial.println(message.pins_count);
  Serial.println(F("I received the following pins: "));
  
  for(int i = 0; i < message.pins_count; i++) {
    printPin(message.pins[i]);
  }

  // if(message.topic == TentacleMessageTopic_action) {
  //   return TentacleMessage::action;
  // }
  //
  // if(message.topic == TentacleMessageTopic_config) {
  //   return TentacleMessage::config;
  // }

  // return TentacleMessage::unknown;
}

void Pseudopod::printPin(const Pin& pin) {
  Serial.print("Protobuf pin: #");
  Serial.print(pin.number);
  Serial.print("\taction:\t");
  Serial.print(pin.action);
  Serial.print("\tvalue:\t");
  Serial.println(pin.value);
  Serial.flush();
}
