#include <FastLED.h>
#include <Arduino.h>
#include <avr/wdt.h>
#include <TimerOne.h>  // Include the TimerOne library
#include "snake.h"
#include "utils.h"
#include <avr/interrupt.h>

snake mySnake = {0, 3, true};

void setup() {
  Serial.begin(9600);

  pinMode(MODE_PIN, INPUT_PULLUP);
  pinMode(INTERACTION_PIN, INPUT_PULLUP);
  pinMode(RESET_PIN, INPUT_PULLUP);

  wdt_enable(WDTO_8S);

  // Initialize the FastLED library
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.clear();
  FastLED.show();

  randomSeed(analogRead(0));

  Timer1.initialize(5000000);  // 5 seconds = 5,000,000 microseconds
  Timer1.attachInterrupt(timer_1_ISR);

  PCICR |= (1 << PCIE2);             // Enable pin change interrupts for PORTD
  PCMSK2 |= (1 << PCINT20);          // Enable pin change interrupt for D4 (PCINT20)
  PCMSK2 |= (1 << PCINT21);          // Enable pin change interrupt for D5 (PCINT21)
  PCMSK2 |= (1 << PCINT22);          // Enable pin change interrupt for D6 (PCINT22)
}

void loop() {
  if (currentMode == SNAKE) {
    run_snake(leds, mySnake);
  }

  wdt_reset();
}

