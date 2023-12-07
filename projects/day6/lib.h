#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char **times;
  char **records;
  size_t len;
} Races;

unsigned long part1(char *input_path);
unsigned long part2(char *input_path);
