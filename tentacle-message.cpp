#include "tentacle-message.h"

TentacleMessage::TentacleMessage(const TentacleMessage::Topic &topic, const std::vector<Pin> &pins) {
  this->topic = topic;
  this->pins = pins;
}

TentacleMessage::Topic TentacleMessage::getTopic() {
  return topic;
}

std::vector<Pin> TentacleMessage::getPins() {
  return pins;
}
