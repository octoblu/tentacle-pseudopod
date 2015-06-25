#ifndef TENTACLE_MESSAGE_H
#define TENTACLE_MESSAGE_H

#include <vector>
#include "pins.hpp"

using namespace std;
class TentacleMessage {

public:
  enum Topic {
    action = 0,
    config = 1,
    unknown = 2
  };

  TentacleMessage(TentacleMessage::Topic topic, vector<Pin> pins);
  TentacleMessage::Topic& getTopic();
  vector<Pin>& getPins();

private:
  TentacleMessage();
  TentacleMessage::Topic topic;
  vector<Pin> pins;
};

#endif
