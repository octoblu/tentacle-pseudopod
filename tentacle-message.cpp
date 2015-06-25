#include "tentacle-message.h"

TentacleMessage::TentacleMessage(TentacleMessage::Topic topic, vector<Pin> pins) {
  this->topic = topic;
  this->pins = pins;
}

TentacleMessage::Topic& TentacleMessage::getTopic() {
  return topic;
}

vector<Pin>& TentacleMessage::getPins() {
  return pins;
}
