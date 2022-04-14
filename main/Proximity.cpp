#ifndef Proximity_cpp
#define Proximity_cpp

#include "RTClib.h"

class Proximity {
  private:
    const int signal_pin = 22;
    
  public:
    Proximity() {
      pinMode(signal_pin, INPUT);
    }

    int get_state() {
      return !digitalRead(signal_pin);
    }
};

#endif
