#ifndef SNAKE_H
#define SNAKE_H

struct snake {
  int head;
  int length;
  bool direction_left;
};

enum block {
  CLEAR,
  APPLE,
  WALL,
  JORMUNGANDR,
  ERROR,
};

void iterate_snake(CRGB leds[], snake& mySnake);

#endif