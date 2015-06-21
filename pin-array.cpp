#include "pin-array.h"

PinArray::PinArray(Pin* elements, size_t length) {
  this->elements = elements;
  this->length = length;
}
