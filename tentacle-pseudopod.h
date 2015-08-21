#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include "pb_encode.h"
#include "pb_decode.h"
}

#include <stddef.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "proto-buf.h"

// Make library cross-compatiable
// with Arduino, GNU C++ for tests, and Spark.
#if defined(ARDUINO) && ARDUINO >= 100
#include "Stream.h"
#elif defined(SPARK)
#include "spark_wiring_stream.h"
#endif

#ifndef BROADCAST_INTERVAL_DEFAULT
#define BROADCAST_INTERVAL_DEFAULT 2000
#endif

class Pseudopod {
  public:
    Pseudopod(Stream& input, Print& output, Tentacle& tentacle);

    size_t authenticate(const char* uuid, const char *token);
    int getBroadcastInterval();
    bool isConfigured();
    bool isConnected();
    TentacleMessageTopic readMessage();
    size_t registerDevice();
    size_t requestConfiguration();
    bool shouldBroadcastPins();
    size_t sendConfiguredPins();
    size_t sendPins(Tentacle::Action* pinActions);
    size_t sendPins();

   private:
    unsigned int broadcastInterval;
    bool broadcastPins;
    bool configured;
    TentacleMessage currentMessage;
    Action* messagePinActions;
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;
    Tentacle* tentacle;

    void resetPinActions();

    static bool customDataDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
    static Action fromTentacleAction(Tentacle::Action action);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static Tentacle::Action toTentacleAction(Action action);
};

#endif
