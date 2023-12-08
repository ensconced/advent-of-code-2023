#pragma once
#include <stddef.h>

typedef struct {
  char *cards;
  int bid;
} Hand;

typedef struct {
  Hand *hands;
  size_t hands_len;
} Hands;

typedef enum {
  HIGH_CARD,
  ONE_PAIR,
  TWO_PAIR,
  THREE_OF_A_KIND,
  FULL_HOUSE,
  FOUR_OF_A_KIND,
  FIVE_OF_A_KIND,
} HandKind;

int part1(char *input_path);
