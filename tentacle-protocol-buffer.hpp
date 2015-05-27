#include "protocols/microblu.pb.h"
#include "tentacle.hpp"

namespace tentacle {
  class TentacleProtoBuf {
;
  public:
    static MicrobluState getStateMessage(Tentacle tentacle);
  };
}
