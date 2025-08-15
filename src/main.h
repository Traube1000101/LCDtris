#ifndef MAIN_H
#define MAIN_H

#include <OneButton.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <bitset>

// Define functions
bool get_block(int, int);
void set_block(int, int, bool);
void initLcd();
void render();
void randomizeBoard(float);
inline char bitsToChar(bool, bool);
void lcdWriteBlock(int, int, char);

#define WIDTH 8
#define HEIGHT 20
#define BACKGROUND 32 // Space char
#define FULL_BLOCK 255
#define LEFT_BLOCK 0
#define RIGHT_BLOCK 1

extern std::bitset<WIDTH * HEIGHT> board;

extern char shadow[WIDTH / 2][HEIGHT];

extern LiquidCrystal_I2C lcd;

extern uint8_t blocks[][8];

#endif
