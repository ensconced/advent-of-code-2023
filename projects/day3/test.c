#include "./lib.h"
#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

bool positioned_numbers_are_equal(PositionedNumber a, PositionedNumber b) {
  return a.num == b.num && a.start_coords.x == b.start_coords.x &&
         a.start_coords.y == b.start_coords.y && a.str_len == b.str_len;
}

bool coordinates_are_equal(Coordinates a, Coordinates b) {
  return a.x == b.x && a.y == b.y;
}

void test_parse_input(void) {
  Grid grid = parse_input("./inputs/part1_example.txt");
  static const size_t expected_number_count = 10;
  PositionedNumber expected_numbers[expected_number_count] = {
      (PositionedNumber){.num = 467,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 0, .y = 0}},
      (PositionedNumber){.num = 114,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 5, .y = 0}},
      (PositionedNumber){.num = 35,
                         .str_len = 2,
                         .start_coords = (Coordinates){.x = 2, .y = 2}},
      (PositionedNumber){.num = 633,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 6, .y = 2}},
      (PositionedNumber){.num = 617,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 0, .y = 4}},
      (PositionedNumber){.num = 58,
                         .str_len = 2,
                         .start_coords = (Coordinates){.x = 7, .y = 5}},
      (PositionedNumber){.num = 592,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 2, .y = 6}},
      (PositionedNumber){.num = 755,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 6, .y = 7}},
      (PositionedNumber){.num = 664,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 1, .y = 9}},
      (PositionedNumber){.num = 598,
                         .str_len = 3,
                         .start_coords = (Coordinates){.x = 5, .y = 9}},
  };

  for (size_t i = 0; i < grid.numbers_len; ++i) {
    assert(positioned_numbers_are_equal(grid.numbers[i], expected_numbers[i]));
  }

  static const size_t expected_symbol_count = 6;
  Coordinates expected_symbols[expected_symbol_count] = {
      (Coordinates){.x = 3, .y = 1}, (Coordinates){.x = 6, .y = 3},
      (Coordinates){.x = 3, .y = 4}, (Coordinates){.x = 5, .y = 5},
      (Coordinates){.x = 3, .y = 8}, (Coordinates){.x = 5, .y = 8},
  };
  for (size_t i = 0; i < expected_symbol_count; ++i) {
    assert(coordinates_are_equal(grid.symbols[i], expected_symbols[i]));
  }
}

int main(void) { test_parse_input(); }
