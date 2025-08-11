#include "OneButton.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

void blinkOn(int, bool);
void printPattern();

OneButton *button[] = {new OneButton(D0, true, true),
                       new OneButton(D1, true, true)};

LiquidCrystal_I2C lcd(0x27, 20, 4);

bool buttonToggle = false;

#define LEFT_BLOCK 0
#define RIGHT_BLOCK 1
#define DOUBLE_BLOCK 255
#define BLOCKS(x, y) x, x, x, x, y, y, y, y
uint8_t blocks[][8] = {{BLOCKS(0x0, 0xff)}, {BLOCKS(0xff, 0x0)}};

void setup() {
  button[0]->attachClick([]() { buttonToggle ^= 1; });
  button[1]->attachClick([]() { lcd.clear(); });

  pinMode(LED_BUILTIN, OUTPUT);

  lcd.init();
  lcd.backlight();

  // Create left & right block chars
  for (int i = 0; i < 2; i++) {
    lcd.createChar(i, blocks[i]);
  }
}

void blinkOn(int ledPin, bool shouldBlink) {
  digitalWrite(ledPin, !(shouldBlink && (millis() / 500) % 2));
}

void loop() {
  button[0]->tick();
  button[1]->tick();
  blinkOn(LED_BUILTIN, buttonToggle);

  printPattern();
  lcd.clear();
}

// Prints alternating blocks filling the screen - test demo
void printPattern() {
  for (int row = 0; row < 4; row++) {
    lcd.setCursor(0, row);
    for (int col = 0; col < 20; col++) {
      uint8_t charToPrint = (col == 0 || col == 19) ? DOUBLE_BLOCK : (col % 2);
      lcd.write(charToPrint);
      delay(50);
    }
    delay(50);
  }
}
