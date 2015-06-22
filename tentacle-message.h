#ifndef TENTACLE_MESSAGE_H
#define TENTACLE_MESSAGE_H

#include "pin-array.h"
#include "pins.hpp"

class TentacleMessage {

public:
  enum Topic {
    action = 0,
    config = 1,
    unknown = 2
  };

  TentacleMessage(const TentacleMessage::Topic &topic, const PinArray& pins);
  const TentacleMessage::Topic& getTopic() const;
  const PinArray& getPins() const;

private:
  const TentacleMessage::Topic *topic;
  const PinArray *pins;
};

#endif
