#ifndef UTIL_H
#define UTIL_H

#define NUM_LEDS 150

#define DATA_PIN 2
#define MODE_PIN 4
#define INTERACTION_PIN 3
#define RESET_PIN 8
#define DEBOUNCE_DELAY 100  // Milliseconds for debounce delay

volatile bool buttonPressed = false;
unsigned long lastDebounceTime = 0;  // Timestamp to track debounce
unsigned long debounceDelay = DEBOUNCE_DELAY;

int brightness = 25;

volatile bool timerTriggered = false;

enum mode {
  SPEEDWAY,
  SNAKE,
  COLOR,
};

mode currentMode = SNAKE;

CRGB leds[NUM_LEDS];

block check_next_block(CRGB leds[], snake mySnake);
void printCRGB(const CRGB &color);

#endif