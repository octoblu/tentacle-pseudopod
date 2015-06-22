#ifndef PIN_ARRAY_H
#define PIN_ARRAY_H
#include <stddef.h>
#include "pins.hpp"
struct PinArray {
    PinArray (Pin* elements, size_t length);
    Pin* elements;
    size_t length;
};

#endif
