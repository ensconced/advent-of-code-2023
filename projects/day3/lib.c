#include "./lib.h"
#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t numbers_capacity = 2048;
const size_t symbols_capacity = 2048;
const size_t number_buffer_capacity = 16;

Grid parse_input(char *input_path) {
  Symbol *symbols = malloc(symbols_capacity * sizeof(Symbol));
  PositionedNumber *numbers =
      malloc(numbers_capacity * sizeof(PositionedNumber));

  size_t symbols_len = 0;
  size_t numbers_len = 0;

  FileLines file_lines = read_file_lines(input_path);
  for (size_t line_idx = 0; line_idx < file_lines.line_count; ++line_idx) {
    char *original_line_pointer = file_lines.lines[line_idx];
    char *movable_line_pointer = original_line_pointer;
    while (1) {
      size_t x = (size_t)(movable_line_pointer - original_line_pointer);

      char number_buffer[number_buffer_capacity];
      maybe_take_numeric_string(&movable_line_pointer, number_buffer,
                                number_buffer_capacity);
      if (strlen(number_buffer)) {
        PositionedNumber positioned_number = {
            .num = atoi(number_buffer),
            .str_len = strlen(number_buffer),
            .start_coords = (Coordinates){.x = x, .y = line_idx},
        };
        if (numbers_len == numbers_capacity - 1) {
          printf("numbers buffer is full\n");
          exit(EXIT_FAILURE);
        }
        numbers[numbers_len++] = positioned_number;
      } else {
        char next_char = movable_line_pointer[0];
        size_t next_char_pos =
            (size_t)(movable_line_pointer - original_line_pointer);
        if (next_char == '\0' || next_char == '\n') {
          break;
        }
        movable_line_pointer++;
        if (next_char != '.') {
          Symbol symbol = {
              .coordinates =
                  (Coordinates){
                      .x = next_char_pos,
                      .y = line_idx,

                  },
              .character = next_char,
          };
          if (symbols_len == symbols_capacity - 1) {
            printf("symbols buffer is full\n");
            exit(EXIT_FAILURE);
          }
          symbols[symbols_len++] = symbol;
        }
      }
    }
  }
  free_file_lines(file_lines);
  return (Grid){.numbers = numbers,
                .numbers_len = numbers_len,
                .symbols = symbols,
                .symbols_len = symbols_len};
}

bool number_is_adjacent_to_symbol(PositionedNumber number, Symbol symbol) {
  size_t box_min_x = number.start_coords.x == 0 ? 0 : number.start_coords.x - 1;
  size_t box_max_x = number.start_coords.x + number.str_len + 1;
  size_t box_min_y = number.start_coords.y == 0 ? 0 : number.start_coords.y - 1;
  size_t box_max_y = number.start_coords.y + 2;
  return symbol.coordinates.x >= box_min_x &&
         symbol.coordinates.x < box_max_x &&
         symbol.coordinates.y >= box_min_y && symbol.coordinates.y < box_max_y;
}

bool is_part_number(PositionedNumber number, Grid grid) {
  for (size_t i = 0; i < grid.symbols_len; ++i) {
    if (number_is_adjacent_to_symbol(number, grid.symbols[i])) {
      return true;
    }
  }
  return false;
}

int sum_part_numbers(Grid grid) {
  int sum = 0;
  for (size_t i = 0; i < grid.numbers_len; ++i) {
    if (is_part_number(grid.numbers[i], grid)) {
      sum += grid.numbers[i].num;
    }
  }
  return sum;
}

int gear_ratio(Symbol symbol, Grid grid) {
  if (symbol.character != '*')
    return 0;
  int result = 1;
  int adjacent_words = 0;
  for (size_t i = 0; i < grid.numbers_len; ++i) {
    if (number_is_adjacent_to_symbol(grid.numbers[i], symbol)) {
      if (adjacent_words == 2)
        return 0;
      result *= grid.numbers[i].num;
      adjacent_words++;
    }
  }
  return adjacent_words == 2 ? result : 0;
}

int sum_gear_ratios(Grid grid) {
  int result = 0;
  for (size_t i = 0; i < grid.symbols_len; ++i) {
    result += gear_ratio(grid.symbols[i], grid);
  }
  return result;
}

void free_grid(Grid grid) {
  free(grid.numbers);
  free(grid.symbols);
}
