#ifndef TENTACLE_MESSAGE_H
#define TENTACLE_MESSAGE_H

#include <vector>
#include "pins.hpp"

class TentacleMessage {

public:
  enum Topic {
    action = 0,
    config = 1
  };

  TentacleMessage(const TentacleMessage::Topic &topic, const std::vector<Pin> &pins);
  TentacleMessage::Topic getTopic();
  std::vector<Pin> getPins();

private:
  TentacleMessage::Topic topic;
  std::vector<Pin> pins;
};
#endif
