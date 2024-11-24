#include <Arduino.h>
#include "snake.h"
#include "utils.h"

block check_next_block(CRGB leds[], snake mySnake) {
  int next_block_index = -1;
  if (mySnake.direction_left == true) {
    next_block_index = (mySnake.head + 1) % 150;
  } else {
    next_block_index = mySnake.head - 1;
    if (next_block_index < 0) {
      next_block_index += 150;
    }
  }

  if ((leds[next_block_index].r == 0) && (leds[next_block_index].g == 0) && (leds[next_block_index].b == 0)) {
    return CLEAR;
  } else if ((leds[next_block_index].r == brightness) && (leds[next_block_index].g == 0) && (leds[next_block_index].b == 0)) {
    return APPLE;
  } else if ((leds[next_block_index].r == 0) && (leds[next_block_index].g == 0) && (leds[next_block_index].b == brightness)) {
    return WALL;
  } else if ((leds[next_block_index].r == 0) && (leds[next_block_index].g > 0) && (leds[next_block_index].b == 0)) {
    return JORMUNGANDR;
  } else {
    Serial.println("error: block not found");
    return ERROR;
  }
}

void printCRGB(const CRGB &color) {
  Serial.print("R: ");
  Serial.print(color.r); // Red component
  Serial.print(", G: ");
  Serial.print(color.g); // Green component
  Serial.print(", B: ");
  Serial.println(color.b); // Blue component
}

void onModeButtonPress() {

  currentMode = (currentMode + 1) % 3;
}

void onInteractionButtonPress() {
  if (debounce_button()) {
    if (currentMode == SPEEDWAY) {

    } else if (currentMode == SNAKE) {
      add_apple(leds);
      add_wall(leds);
    } else {

    }
  }
}

void onResetButtonPress() {
  wdt_enable(WDTO_15MS);
}

bool debounce_button() {
  unsigned long currentTime = millis();

  // Only register a press if the debounce delay has passed
  if (currentTime - lastDebounceTime > debounceDelay) {
    lastDebounceTime = currentTime; // Update the last debounce time
    return true;
  } else {
    return false;
  }
}

void sparkle(CRGB leds[]) {
  while (count_blank(leds) != 0) {
    // Randomly select an LED to sparkle
    int randomIndex = random(NUM_LEDS);

    // Light up the selected LED with green
    leds[randomIndex] = CRGB(0, brightness, 0);
    FastLED.show();
    delay(1);  // Short delay for the sparkle

  }
}

int count_blank(CRGB leds[]) {
  int count = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((leds[i].r == 0) && (leds[i].g == 0) && (leds[i].b == 0)) {
      count++;
    }
  }
  return count;
}

void timer_1_ISR() {
  timerTriggered = true;  // Set the flag to true when the timer triggers
}