#include "pin-array.h"
#include <stddef.h>

PinArray::PinArray(size_t length) {
  this->elements = new Pin[length];
  this->length = length;
  resetPins();
}

PinArray::~PinArray() {
  if(elements == NULL) {
    return;
  }

  delete[] elements;
}

Pin* PinArray::getElements() const {
  return elements;
}

size_t PinArray::getLength() const {
  return length;
}

PinArray PinArray::update(Pin* pins, size_t length) {
  for(int i = 0; i < length; i++) {
    Pin pin = pins[i];
    this->elements[pin.getNumber()] = pin;
  }
  return *this;
}

PinArray PinArray::resetPins() {
  for(int i = 0; i < length; i++) {
    elements[i].reset(i);
  }
  
  return *this;
}

Pin& PinArray::operator[] (const int index)
{
    return elements[index];
}
