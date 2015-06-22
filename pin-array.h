#ifndef PIN_ARRAY_H
#define PIN_ARRAY_H
#include <stddef.h>
#include "pins.hpp"
class PinArray {
  public:
      PinArray (size_t length=0);
      ~PinArray();

      size_t getLength() const;
      Pin* getElements() const;
      PinArray update(Pin* pins, size_t length);
      PinArray resetPins();

      Pin& operator[] (const int index);

  private:
      Pin* elements;
      size_t length;
};

#endif
