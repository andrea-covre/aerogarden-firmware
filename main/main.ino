// Libraries
#include <Wire.h>

// Modules
#include "LCD.cpp"
#include "RTC.cpp"
#include "DHT11.cpp"
#include "Proximity.cpp"

Rtc rtc;
Dht11 dht;
Lcd screen = Lcd();
Proximity proximity = Proximity();

void setup() {
  // COMS
  Wire.begin();
  Serial.begin(9600);
  
  // Devices
  rtc.init();
  screen.init(rtc, dht);
}

void loop() { 
  screen.update();
}
