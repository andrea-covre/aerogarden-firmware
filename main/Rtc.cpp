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
        Serial.println("Couldn't find RTC");
        while (1);
      }
      if (!rtc.isrunning()) {
        // Reset RTC time from machine timestamp
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      }
    };
 
    int get_time() {
      DateTime now = rtc.now();
      Serial.println(now.unixtime());
      return 2;
    }

    int get_ts() {
      DateTime now = rtc.now();
      return now.unixtime();
    }
};

#endif
