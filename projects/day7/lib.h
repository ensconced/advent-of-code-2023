#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char *cards;
  char *sorted_cards;
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

bool has_n_runs(char *cards, size_t target_run_count, size_t target_run_len);
int part1(char *input_path);
