#pragma once
#include <stddef.h>

typedef struct {
  int idx;
  size_t count;
  int *winning_numbers;
  size_t winning_numbers_len;
  int *my_numbers;
  size_t my_numbers_len;
} Card;

int part1(char *input_path);
int part2(char *input_path);
