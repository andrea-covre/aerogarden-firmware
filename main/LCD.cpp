#ifndef LCD_cpp
#define LCD_cpp

// Libraries
#include <LiquidCrystal.h>

// Modules
#include "RTC.cpp"
#include "DHT11.cpp"
#include "Proximity.cpp"

class Lcd {
  private:
    // PINS
    const int RS = 2;
    const int E = 3;
    const int D4 = 28;
    const int D5 = 30;
    const int D6 = 32;
    const int D7 = 34;
    const int LED = 8;

    // Screen properties
    const int width = 16;
    const int height = 2;
    LiquidCrystal *lcd;

    // Other devices used;
    Rtc rtc;
    Dht11 dht;
    Proximity proximity;

    // Animations variables
    int const bootup_view_time = 5;     // s
    int const dim_down_time = 30;       // s
    int const dim_down_step_time = 20;  // ms
    int brightness = 255;
    int proximity_state;
    uint32_t led_dim_ts;                // s
    uint32_t state_switch_ts;           // s
    uint32_t view_update_ts;            // s
    uint32_t ts;                        // s

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

    // Droplet char
    struct specialChar water_drop = {1, {
        0b00000,
        0b00100,
        0b01110,
        0b01110,
        0b11111,
        0b11111,
        0b11111,
        0b01110
      }
    };

    // thermo char
    struct specialChar thermo = {2, {
        0b00100,
        0b01010,
        0b01010,
        0b01010,
        0b01110,
        0b11111,
        0b11111,
        0b01110
      }
    };

    // minitree char
    struct specialChar minitree = {3, {
        0b01110,
        0b11111,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b10101,
        0b01110
      }
    };

    // leaf char
    struct specialChar leaf = {4, {
        0b00100,
        0b01110,
        0b01110,
        0b11111,
        0b11111,
        0b01110,
        0b00100,
        0b00100
      }
    };

    // clock char
    struct specialChar clock_char = {5, {
        0b00000,
        0b00000,
        0b01110,
        0b10101,
        0b10111,
        0b10001,
        0b01110,
        0b00000
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
      pinMode(LED, OUTPUT);
      digitalWrite(LED, HIGH);

      // Upload special chars
      lcd->createChar(flower.index, flower.charMap);
      lcd->createChar(water_drop.index, water_drop.charMap);
      lcd->createChar(thermo.index, thermo.charMap);
      lcd->createChar(minitree.index, minitree.charMap);
      lcd->createChar(leaf.index, leaf.charMap);
      lcd->createChar(clock_char.index, clock_char.charMap);

      // Start state machine
      state = RESET;
    };

    void init(Rtc rtc_obj, Dht11 dht_obj) {
      
      // Link other modules
      dht = dht_obj;
      rtc = rtc_obj;

      // Get current timestamp
      ts = rtc.get_ts();
      state_switch_ts = ts;
      view_update_ts = ts;
    }

    State get_state() {
      return state;
    }

    void print_time() {
      lcd->setCursor(0,0);
      lcd->write(clock_char.index);
      lcd->print(rtc.get_time());
    }

    void print_temp() {
      lcd->setCursor(7,0);
      lcd->write(thermo.index);
      lcd->print((String) (int) dht.get_temperature() + "C");
    }

    void print_hum() {
      lcd->setCursor(12,0);
      lcd->write(water_drop.index);
      lcd->print((String) (int) dht.get_humidity() + "%");
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
      for (int i = 0; i < width; i+=2) {
        lcd->setCursor(i,1);
        lcd->write(minitree.index);
        lcd->setCursor(i+1,1);
        lcd->write(leaf.index);
      }
    };

    void default_led_on_view() {
      lcd->clear();
      print_time();
      print_temp();
      print_hum();
    };

    
  // State machine
  public:void update() {
    // Updating values
    ts = rtc.get_ts();
    proximity_state = proximity.get_state();
    
    switch(state) {
      case RESET:
        bootup_view();
        state = BOOTUP;
        state_switch_ts = ts;
        break;
        
      case BOOTUP:
        if (ts - state_switch_ts > bootup_view_time) {
          state = DEF_LED_ON;
          state_switch_ts = ts;
        }
        break;

      case DEF_LED_ON:
        if (ts > view_update_ts) {
          default_led_on_view();
          view_update_ts = ts;
        }

        if (ts - state_switch_ts > dim_down_time) {
          state = DEF_LED_OFF;
          state_switch_ts = ts;
          led_dim_ts = millis();
        }

        if (proximity_state) {
          state_switch_ts = ts;
        }
        break;

      case DEF_LED_OFF:
        if (ts > view_update_ts) {
          default_led_on_view();
          view_update_ts = ts;
        }
        if (millis() - led_dim_ts > dim_down_step_time and brightness > 0) {
          brightness -= 1;
          analogWrite(LED, brightness);
          led_dim_ts = millis();
        }
        if (proximity_state) {
          state = DEF_LED_ON;
          brightness = 255;
          digitalWrite(LED, HIGH);
          state_switch_ts = ts;
        }
        break;
        
    }
  }
};

#endif
