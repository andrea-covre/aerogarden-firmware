#include <LiquidCrystal.h>

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
      lcd = new LiquidCrystal(RS, E, D4, D5, D6, D7);
      lcd->begin(width, height);
      lcd->createChar(flower.index, flower.charMap);
      this->bootup_screen();
    };

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
