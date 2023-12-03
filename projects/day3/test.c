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

bool symbols_are_equal(Symbol a, Symbol b) {
  return a.coordinates.x == b.coordinates.x &&
         a.coordinates.y == b.coordinates.y && a.character == b.character;
}

void test_parse_input(void) {
  Grid grid = parse_input("./inputs/example.txt");
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
  Symbol expected_symbols[expected_symbol_count] = {
      (Symbol){
          .coordinates = (Coordinates){.x = 3, .y = 1},
          .character = '*',
      },
      (Symbol){
          .coordinates = (Coordinates){.x = 6, .y = 3},
          .character = '#',
      },
      (Symbol){
          .coordinates = (Coordinates){.x = 3, .y = 4},
          .character = '*',
      },
      (Symbol){
          .coordinates = (Coordinates){.x = 5, .y = 5},
          .character = '+',
      },
      (Symbol){
          .coordinates = (Coordinates){.x = 3, .y = 8},
          .character = '$',
      },
      (Symbol){
          .coordinates = (Coordinates){.x = 5, .y = 8},
          .character = '*',
      },
  };
  for (size_t i = 0; i < expected_symbol_count; ++i) {
    assert(symbols_are_equal(grid.symbols[i], expected_symbols[i]));
  }
}

int main(void) {
  test_parse_input();
  Grid example_grid = parse_input("./inputs/example.txt");
  int example_result = sum_part_numbers(example_grid);
  assert(example_result == 4361);

  Grid grid = parse_input("./inputs/input.txt");
  int part1_result = sum_part_numbers(grid);
  printf("part 1 result: %d\n", part1_result);

  int part2_result = sum_gear_ratios(grid);
  printf("part 2 result: %d\n", part2_result);
}
