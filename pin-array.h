#include <stddef.h>
#include "pins.hpp"
struct PinArray {
    PinArray (Pin* elements, size_t length);
    Pin* elements;
    size_t length;
};
