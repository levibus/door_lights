#include <FastLED.h>
#include <Arduino.h>
#include <avr/wdt.h>
#include <TimerOne.h>  // Include the TimerOne library
#include "snake.h"
#include "utils.h"

snake mySnake = {0, 3, true};

void setup() {
  Serial.begin(9600);

  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(INTERACTION_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(MODE_PIN), onModeButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(INTERACTION_PIN), onInteractionButtonPress, RISING);
  attachInterrupt(digitalPinToInterrupt(RESET_PIN), onResetButtonPress, RISING);

  wdt_enable(WDTO_8S);

  // Initialize the FastLED library
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(0));

  Timer1.initialize(5000000);  // 5 seconds = 5,000,000 microseconds
  Timer1.attachInterrupt(timer_1_ISR);
}

void loop() {
  if (currentMode == SNAKE) {
    run_snake(leds, mySnake);
  }

  wdt_reset();
}

