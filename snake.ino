#include "snake.h"

block nextBlock = CLEAR;
bool apple = false;
int next_index = 0;
int apple_count = 0;
int wall_count = 0;

void iterate_snake(CRGB leds[], snake& mySnake);

// if snake going left but tail greater than head, gonna have to handle destroy differently
void run_snake(CRGB leds[], snake& mySnake) {
  if (timerTriggered) {
    timerTriggered = false;
    apple_count = count_apples(leds);
    wall_count = count_walls(leds);
    if (apple_count < 3) {
      add_apple(leds);
    }
    if (wall_count < 3) {
      add_wall(leds);
    }
  }

  // if (apple) {
  //   grow_snake(mySnake);
  //   apple = false;
  // }

  nextBlock = check_next_block(leds, mySnake);
  if (nextBlock == CLEAR) {
    iterate_snake(leds, mySnake, nextBlock);
  } else if (nextBlock == APPLE) {
    apple = true;
    iterate_snake(leds, mySnake, nextBlock); // move to next iteration
    grow_snake(mySnake);
  } else if (nextBlock == WALL) {
    wall_break(leds, mySnake);
    change_snake_direction(mySnake);
  } else if (nextBlock == JORMUNGANDR) {
    FastLED.clear();
    delay(500);
    sparkle(leds);
    onResetButtonPress();
  }
}

void iterate_snake(CRGB leds[], snake& mySnake, block nextBlock) {
  int tail = 0;

  if (mySnake.direction_left) {
    next_index = (mySnake.head + 1) % 150;
    tail = next_index - (mySnake.length - 1);
    if (tail < 0) {
      tail += 150;
    }
  } else {
    next_index = mySnake.head - 1;
    tail = (next_index + (mySnake.length - 1)) % 150;
    if (next_index < 0) {
      next_index += 150;
    }
  }

  for (int color_brightness = 0; color_brightness <= brightness; color_brightness += 2) {
    leds[next_index] = CRGB(0, color_brightness, 0); // Set LED to red with varying brightness
    FastLED.show();
  }
  delay(50);
  if (nextBlock != APPLE) {
    for (int color_brightness = brightness; color_brightness >= 0; color_brightness -= 5) {
      if ((leds[tail].r == 0) && (leds[tail].g > 0) && (leds[tail].b == 0)) {
        leds[tail] = CRGB(0, color_brightness, 0); // Reduce brightness to create fade-out effect
        FastLED.show();
      }
    }
  }
  delay(50);

  // Serial.print("head: ");
  // Serial.println(next_index);
  // Serial.print("tail: ");
  // Serial.println(tail);
  mySnake.head = next_index;
}

void grow_snake(snake& mySnake) {
  mySnake.length++;
}

void change_snake_direction(snake& mySnake) {
  int head = 0;
  if (mySnake.direction_left) {
    head = (mySnake.head + 1) - (mySnake.length - 1);
    if (head < 0) {
      head += 150;
    }
  } else {
    head = ((mySnake.head - 1) + (mySnake.length - 1)) % 150;
  }
  mySnake.head = head;
  mySnake.direction_left = !mySnake.direction_left;
}

void add_apple(CRGB leds[]) {
  bool found_position = false;
  int randomNum = 0;
  while(!found_position) {
    randomNum = random(1, 149); // range is 1-148
    if (leds[randomNum] == CRGB::Black) {
      found_position = true;
    }
  }
  leds[randomNum] = CRGB(brightness, 0, 0);
}

void add_wall(CRGB leds[]) {
  bool found_position = false;
  int randomNum = 0;
  while(!found_position) {
    randomNum = random(1, 149);
    if (leds[randomNum] == CRGB::Black) {
      found_position = true;
    }
  }
  leds[randomNum] = CRGB(0, 0, brightness);
}

void wall_break(CRGB leds[], snake& mySnake) {
  int next_index = 0;
  if (mySnake.direction_left) {
    next_index = mySnake.head + 1; // don't worry about ends because walls won't spawn on ends
  } else {
    next_index = mySnake.head - 1;
  }
  for (int color_brightness = brightness; color_brightness >= 0; color_brightness -= 1) {
    leds[next_index] = CRGB(0, 0, color_brightness); // Reduce brightness to create fade-out effect
    FastLED.show();
  }
  for (int color_brightness = 0; color_brightness <= brightness * 2 / 3; color_brightness += 1) {
    leds[next_index] = CRGB(0, 0, color_brightness); // Set LED to red with varying brightness
    FastLED.show();
  }
  for (int color_brightness = brightness * 2 / 3; color_brightness >= 0; color_brightness -= 1) {
    leds[next_index] = CRGB(0, 0, color_brightness); // Reduce brightness to create fade-out effect
    FastLED.show();
  }
  for (int color_brightness = 0; color_brightness <= brightness / 3; color_brightness += 1) {
    leds[next_index] = CRGB(0, 0, color_brightness); // Set LED to red with varying brightness
    FastLED.show();
  }
  for (int color_brightness = brightness / 3; color_brightness >= 0; color_brightness -= 1) {
    leds[next_index] = CRGB(0, 0, color_brightness); // Reduce brightness to create fade-out effect
    FastLED.show();
  }
  // Ensure the LED is completely off at the end
  leds[next_index] = CRGB::Black;
  FastLED.show();
}

int count_apples(CRGB leds[]) {
  int count = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((leds[i].r > 0) && (leds[i].g == 0) && (leds[i].b == 0)) {
      count++;
    }
  }
  return count;
}

int count_walls(CRGB leds[]) {
  int count = 0;
  for (int i = 0; i < NUM_LEDS; i++) {
    if ((leds[i].r == 0) && (leds[i].g == 0) && (leds[i].b > 0)) {
      count++;
    }
  }
  return count;
}