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
    int const bootup_view_time = 5;
    uint32_t state_switch_ts = 0; 
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

    // State machine
    enum State {
      RESET,
      BOOTUP,
      DEF_LED_ON,
      DEF_LED_OFF
    };

    enum State state;
    
  public:
    Lcd() {
      // Set up screen
      lcd = new LiquidCrystal(RS, E, D4, D5, D6, D7);
      lcd->begin(width, height);

      // Upload special chars
      lcd->createChar(flower.index, flower.charMap);

      // Start state machine
      state = RESET;
    };

    void init(Rtc rtc_obj) {
      // Link RTC and get current timestamp
      rtc = rtc_obj;
      ts = rtc.get_ts();
      state_switch_ts = ts;
    }

    State get_state() {
      return state;
    }

    int print_time() {
      lcd->clear();
      lcd->setCursor(3,0);
      lcd->print("ciao");
      Serial.println(rtc.get_time());
      lcd->print(rtc.get_time());
    }
    
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

  public:void update() {
    switch(state) {
      case RESET:
        bootup_screen();
        state = BOOTUP;
        state_switch_ts = rtc.get_ts();
        break;
        
      case BOOTUP:
        if (rtc.get_ts() - state_switch_ts > bootup_view_time) {
          state = DEF_LED_ON;
          lcd->clear();
        }
        break;
    }
  }
};

#endif
