// Libraries
#include <Wire.h>

// Modules
#include "Lcd.cpp"
#include "Rtc.cpp"

Rtc rtc;
Lcd screen = Lcd(rtc);

void setup() {
  // COMS
  Wire.begin();
  Serial.begin(9600);

  // Devices
  rtc.init();
  rtc.get_time();
}

//----------------- Loop -----------------

void loop() { 

  //Serial.println(rtc.get_time(), DEC);

}
