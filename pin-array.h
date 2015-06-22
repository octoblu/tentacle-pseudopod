#ifndef PIN_ARRAY_H
#define PIN_ARRAY_H
#include <stddef.h>
#include "pins.hpp"
struct PinArray {
    PinArray (Pin* elements=NULL, size_t length=0);
    Pin* elements;
    size_t length;
    Pin& operator[] (const int index);
};

#endif
