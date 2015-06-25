#ifndef TENTACLE_MESSAGE_H
#define TENTACLE_MESSAGE_H

class TentacleMessage {

public:
  enum Topic {
    action = 0,
    config = 1,
    unknown = 2
  };
};

#endif
