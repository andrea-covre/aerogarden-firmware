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

    void print_time() {
      lcd->setCursor(0,0);
      lcd->print(rtc.get_time());
    }

    void print_temp() {
      lcd->setCursor(7,0);
      lcd->print("00C");
    }

    void print_hum() {
      lcd->setCursor(11,0);
      lcd->print("00%");
    }

  // Screen views    
  private:
    void bootup_view() {
      lcd->clear();
      lcd->setCursor(3,0);
      lcd->print("Aerogarden");
      lcd->setCursor(1,0);
      lcd->write(flower.index);
      lcd->setCursor(14,0);
      lcd->write(flower.index);
    };

    void default_led_on_view() {
      lcd->clear();
      Serial.println("ccc");
      print_time();
      print_temp();
      print_hum();
    };

    
  // State machine
  public:void update() {
    switch(state) {
      case RESET:
        bootup_view();
        state = BOOTUP;
        state_switch_ts = rtc.get_ts();
        break;
        
      case BOOTUP:
        if (rtc.get_ts() - state_switch_ts > bootup_view_time) {
          state = DEF_LED_ON;
          state_switch_ts = rtc.get_ts();
        }
        break;

      case DEF_LED_ON:
        if (rtc.get_ts() > state_switch_ts) {
          default_led_on_view();
          state_switch_ts = rtc.get_ts();
        }
        break;
    }
  }
};

#endif
