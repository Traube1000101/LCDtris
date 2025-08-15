#include <OneButton.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <bitset>

using std::bitset;

// Predefine functions
bool get_block(int, int);
void set_block(int, int, bool);
void initLcd();
void render();
void randomizeBoard(float);
inline char bitsToChar(bool, bool);
void lcdWriteBlock(int, int, char);

#define WIDTH 8
#define HEIGHT 20
LiquidCrystal_I2C lcd(0x27, HEIGHT, WIDTH / 2);

#define BACKGROUND 32 // Space char
#define FULL_BLOCK 255
// Custom left & right block chars
#define LEFT_BLOCK 0
#define RIGHT_BLOCK 1
#define BLOCKS(x, y) x, x, x, x, y, y, y, y
uint8_t blocks[][8] = {{BLOCKS(0x0, 0xff)}, {BLOCKS(0xff, 0x0)}};

// Bitset to store every block
bitset<WIDTH * HEIGHT> board;
bool get_block(int x, int y) { return board[y * WIDTH + x]; }
void set_block(int x, int y, bool value) { board[y * WIDTH + x] = value; }

OneButton *button[] = {new OneButton(D0, true, true),
                       new OneButton(D1, true, true)};

// Buffer holding last written chars
char shadow[WIDTH / 2][HEIGHT];

void setup() {
  initLcd();

  // Create left & right block chars
  for (int i = 0; i < 2; i++) {
    lcd.createChar(i, blocks[i]);
    delay(50);
  }
}

void setup1() {
  button[0]->attachClick([]() { randomizeBoard(0.05); });
  button[1]->attachClick([]() { board.reset(); });
}

// Delays in milliseconds
#define RENDER_DELAY 100
#define PHYSICS_DELAY 1000

void loop() {
  static unsigned long lastMicrosRender = 0, lastMicrosPhysics = 0;

  if (unsigned long currentMicros = millis();
      currentMicros - lastMicrosPhysics >= PHYSICS_DELAY) {
    randomizeBoard(0.05);
    lastMicrosPhysics = currentMicros;
  }
  if (unsigned long currentMicros = millis();
      currentMicros - lastMicrosRender >= RENDER_DELAY) {
    render();
    lastMicrosRender = currentMicros;
  }
}

void loop1() {
  button[0]->tick();
  button[1]->tick();
}

void initLcd() {
  lcd.init();
  lcd.backlight();
  // Initialize with spaces
  for (int r = 0; r < WIDTH / 2; ++r)
    for (int c = 0; c < HEIGHT; ++c)
      shadow[r][c] = ' ';
}

void render() {
  for (int row = 0; row < 4; ++row) {
    int xLeft = row * 2;
    int xRight = xLeft + 1;
    for (int col = 0; col < 20; ++col) {
      int y = col;
      bool leftBit = (xLeft < WIDTH && y < HEIGHT) ? get_block(xLeft, y) : false;
      bool rightBit =
          (xRight < WIDTH && y < HEIGHT) ? get_block(xRight, y) : false;
      char out = bitsToChar(leftBit, rightBit);
      lcdWriteBlock(y, xLeft / 2, out);
    }
  }
}

void randomizeBoard(float fillProbability) {
  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    float r = random(1000) / 1000.0;
    if (r < fillProbability) {
      if (random(2) == 0)
      board[i] = true;
    }
  }
}

// Convert left & right bits to single char
inline char bitsToChar(bool leftBit, bool rightBit) {
  static constexpr char lut[4] = {32, LEFT_BLOCK, RIGHT_BLOCK, FULL_BLOCK};
  return lut[(leftBit << 1) | rightBit];
}

void lcdWriteBlock(int col, int row, char block) {
  // Write to lcd if within lcd bounds & char has changes
  if (col < 0 || col >= HEIGHT || row < 0 || row >= WIDTH / 2 ||
      shadow[row][col] == block)
    return;
  lcd.setCursor(col, row);
  lcd.write(block);
  shadow[row][col] = block;
}
