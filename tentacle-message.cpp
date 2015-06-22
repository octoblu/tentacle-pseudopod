#include "tentacle-message.h"

TentacleMessage::TentacleMessage(const TentacleMessage::Topic &topic, const PinArray &pins) {
  this->topic = &topic;
  this->pins = &pins;
}

const TentacleMessage::Topic& TentacleMessage::getTopic() const {
  return *topic;
}

const PinArray& TentacleMessage::getPins() const {
  return *pins;
}
