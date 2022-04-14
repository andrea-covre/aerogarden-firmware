// Libraries
#include <Wire.h>

// Modules
#include "LCD.cpp"
#include "RTC.cpp"
#include "DHT11.cpp"

Rtc rtc;
Dht11 dht;
Lcd screen = Lcd();

void setup() {
  // COMS
  Wire.begin();
  Serial.begin(9600);
  
  // Devices
  rtc.init();
  screen.init(rtc, dht);
}

//----------------- Loop -----------------

void loop() { 
  screen.update();
}
