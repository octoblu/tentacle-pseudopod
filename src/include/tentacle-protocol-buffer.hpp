#include "microblu.pb.h"
#include "tentacle.hpp"

namespace tentacle {
  class TentacleProtoBuf {
  public:
    _MicrobluState getStateMessage(Tentacle tentacle);
  };
}
