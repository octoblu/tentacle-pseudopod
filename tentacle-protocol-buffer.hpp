#include "protocols/microblu.pb.h"
#include "tentacle.hpp"

namespace tentacle {
  class TentacleProtoBuf {
;
  public:
    static _MicrobluState getStateMessage(Tentacle tentacle);
  };
}
