#ifndef RTC_cpp
#define RTC_cpp

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
 
    String get_time(bool with_seconds = false) {
      DateTime now = rtc.now();
      String h = (String) now.hour();
      String m = (String) now.minute();
      String s = (String) now.second();

      if (h.length() < 2) {h = "0" + h;}
      if (m.length() < 2) {m = "0" + m;}
      if (s.length() < 2) {s = "0" + s;}
      
      if (with_seconds) {
        return h + ":" + m + ":" + s;
      }
      return h + ":" + m;
    }

    uint32_t get_ts() {
      DateTime now = rtc.now();
      return now.unixtime();
    }
};

#endif
