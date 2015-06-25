#ifndef TENTACLE_PSEUDOPOD_H
#define TENTACLE_PSEUDOPOD_H

extern "C" {
#include <pb_encode.h>
#include <pb_decode.h>
}

#include <stddef.h>

#include "pb_arduino_encode.h"
#include "pb_arduino_decode.h"

#include "tentacle.h"
#include "Stream.h"
#include "proto-buf.hpp"

class Pseudopod {
  public:
    Pseudopod(Stream& input, Print& output, Tentacle& tentacle);

    size_t sendConfiguredPins();
    size_t sendPins(Action* pinActions);
    size_t sendPins();

    TentacleMessageTopic readMessage();

    size_t authenticate(const char* uuid, const char *token);
    size_t registerDevice();

   private:
    pb_ostream_t pbOutput;
    pb_istream_t pbInput;
    Tentacle* tentacle;
    Action* messagePinActions;

    void resetPinActions();
    static void printPin(const Pin& pin);

    TentacleMessage currentMessage;

    static bool pinEncode(pb_ostream_t *stream, const pb_field_t *field, void * const *arg);
    static bool pinDecode(pb_istream_t *stream, const pb_field_t *field, void **arg);
};

#endif
