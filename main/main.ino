// Libraries
#include <Wire.h>

// Modules
#include "Lcd.cpp"
#include "Rtc.cpp"
#include "DHT11.cpp"

Rtc rtc;
Lcd screen = Lcd();
Dht11 dht;

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
  screen.update();
}
