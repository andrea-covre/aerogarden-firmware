// Libraries
#include <Wire.h>

// Modules
#include "Lcd.cpp"
#include "Rtc.cpp"

Rtc rtc;
Lcd screen = Lcd();

void setup() {
  // COMS
  Wire.begin();
  Serial.begin(9600);
  
  // Devices
  rtc.init();
  screen.init(rtc);
}

//----------------- Loop -----------------

void loop() { 

}
