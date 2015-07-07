#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include "arduino-nanopb/pb_encode.h"
#include "arduino-nanopb/pb_decode.h"
}

#include <stddef.h>

#include "arduino-nanopb/pb_arduino_encode.h"
#include "arduino-nanopb/pb_arduino_decode.h"

#include "tentacle/tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"

#ifndef BROADCAST_INTERVAL_DEFAULT
#define BROADCAST_INTERVAL_DEFAULT 2000
#endif

class Pseudopod {
  public:
    Pseudopod(Stream& input, Print& output, Tentacle& tentacle);

    size_t sendConfiguredPins();
    size_t sendPins(Action* pinActions);
    size_t sendPins();

    TentacleMessageTopic readMessage();
    int getBroadcastInterval();
    bool shouldBroadcastPins();
    bool isConfigured();
    size_t requestConfiguration();
    size_t authenticate(const char* uuid, const char *token);
    size_t registerDevice();

   private:
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;
    Tentacle* tentacle;
    Action* messagePinActions;
    bool broadcastPins = false;
    bool configured = false;
    unsigned int broadcastInterval = BROADCAST_INTERVAL_DEFAULT;
    void resetPinActions();
    static void printPin(const Pin& pin);

    TentacleMessage currentMessage;

    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
};

#endif
