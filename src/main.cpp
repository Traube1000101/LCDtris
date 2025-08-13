#include "OneButton.h"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <bitset>

using std::bitset;

// Predefine functions
bool get_cell(int, int);
void set_cell(int, int, bool);
void render();
void randomizeBoard(float);

#define WIDTH 8
#define HEIGHT 20
LiquidCrystal_I2C lcd(0x27, HEIGHT, WIDTH / 2);

#define LEFT_BLOCK 0
#define RIGHT_BLOCK 1
#define DOUBLE_BLOCK 255
#define BLOCKS(x, y) x, x, x, x, y, y, y, y
uint8_t blocks[][8] = {{BLOCKS(0x0, 0xff)}, {BLOCKS(0xff, 0x0)}};

bitset<WIDTH * HEIGHT> board;
bool get_cell(int x, int y) { return board[y * WIDTH + x]; }
void set_cell(int x, int y, bool value) { board[y * WIDTH + x] = value; }

OneButton *button[] = {new OneButton(D0, true, true),
                       new OneButton(D1, true, true)};

void setup() {
  lcd.init();
  lcd.backlight();

  // Create left & right block chars
  for (int i = 0; i < 2; i++) {
    lcd.createChar(i, blocks[i]);
    delay(50);
  }

  button[0]->attachClick([]() {
    randomizeBoard(0.5);
    render();
  });
  button[1]->attachClick([]() { lcd.clear(); });

  pinMode(LED_BUILTIN, OUTPUT);

  randomizeBoard(0.5);
  render();
}

void loop() {
  button[0]->tick();
  button[1]->tick();
}

void render() {
  for (int row = 0; row < 4; ++row) {
    lcd.setCursor(0, row);

    int xLeft = row * 2;
    int xRight = xLeft + 1;

    for (int col = 0; col < 20; ++col) {

      int y = col;

      bool leftBit = (xLeft < WIDTH && y < HEIGHT) ? get_cell(xLeft, y) : false;
      bool rightBit =
          (xRight < WIDTH && y < HEIGHT) ? get_cell(xRight, y) : false;

      char charToPrint;

      if (!leftBit && !rightBit) {
        charToPrint = ' ';
      } else if (leftBit && !rightBit) {
        charToPrint = LEFT_BLOCK;
      } else if (!leftBit && rightBit) {
        charToPrint = RIGHT_BLOCK;
      } else {
        charToPrint = DOUBLE_BLOCK;
      }

      lcd.write(charToPrint);
    }
  }
}

void randomizeBoard(float fillProbability = 0.5) {
  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    float r = random(1000) / 1000.0;
    if (r < fillProbability) {
      bool val = random(2);
      board[i] = val;
    } else {
      board[i] = 0;
    }
  }
}
