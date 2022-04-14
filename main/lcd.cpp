#ifndef Lcd_cpp
#define Lcd_cpp

// Libraries
#include <LiquidCrystal.h>

// Modules
#include "Rtc.cpp"

class Lcd {
  private:
    // PINS
    const int RS = 2;
    const int E = 3;
    const int D4 = 4;
    const int D5 = 5;
    const int D6 = 6;
    const int D7 = 7;

    // Screen properties
    const int width = 16;
    const int height = 2;
    LiquidCrystal *lcd;

    // Other devices used;
    Rtc rtc;

    // Animations variables
    uint32_t ts;

    // Special chars struct
    struct specialChar {
      int index;
      uint8_t charMap[8];
    };

    // Flower char
    struct specialChar flower = {0, {
        0b10101,
        0b11111,
        0b01110,
        0b00100,
        0b00101,
        0b10101,
        0b01110,
        0b00100
      }
    };
    
  public:
    Lcd() {
      // Set up screen
      lcd = new LiquidCrystal(RS, E, D4, D5, D6, D7);
      lcd->begin(width, height);

      // Upload special chars
      lcd->createChar(flower.index, flower.charMap);

      // Start state machine
      this->bootup_screen();
    };

    void init(Rtc rtc_obj) {
      // Link RTC and get current timestamp
      rtc = rtc_obj;
      Serial.println("pre");
      ts = rtc.get_ts();
      Serial.println("succ");
    }

    int print_time() {
      lcd->clear();
      lcd->setCursor(3,0);
      lcd->print("ciao");
      Serial.println(rtc.get_time());
      lcd->print(rtc.get_time());
    }

    void update() {
      
    };
    
  private:
    void bootup_screen() {
      lcd->clear();
      lcd->setCursor(3,0);
      lcd->print("Aerogarden");
      lcd->setCursor(1,0);
      lcd->write(flower.index);
      lcd->setCursor(14,0);
      lcd->write(flower.index);
    };
};

#endif
