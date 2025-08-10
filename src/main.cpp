#include "OneButton.h"
#include <Arduino.h>

void blinkOn(int, bool);

OneButton button1(D0, true, true);
OneButton button2(D1, true, true);

bool buttonToggle = false;

void setup() {
  Serial.begin(9600);

  button1.attachClick([]() { buttonToggle ^= 1; });
  button2.attachClick([]() { Serial.println("OwO"); });

  pinMode(LED_BUILTIN, OUTPUT);
}

void blinkOn(int ledPin, bool shouldBlink) {
  digitalWrite(ledPin, !(shouldBlink && (millis() / 500) % 2));
}

unsigned long previousMillis = 0;
void loop() {
  button1.tick();
  button2.tick();
  blinkOn(LED_BUILTIN, buttonToggle);
}
