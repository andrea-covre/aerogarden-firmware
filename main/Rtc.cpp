#ifndef Rtc_cpp
#define Rtc_cpp

#include <Wire.h>
#include "RTClib.h"

class Rtc {
  private:
    RTC_DS1307 rtc;
    
  public:
    void init() {
      if (! rtc.begin()) {
        Serial.println("RTC Module cound NOT be found!");
        while (1);
      }
      if (!rtc.isrunning()) {
        // Reset RTC time from machine timestamp
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }
    };
 
    String get_time() {
      DateTime now = rtc.now();
      return (String)now.hour() + ":" + (String)now.minute() + ":" + (String)now.second();
    }

    uint32_t get_ts() {
      DateTime now = rtc.now();
      return now.unixtime();
    }
};

#endif
