#include "./lib.h"
#include "../file_utils/lib.h"
#include "../parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t NUMBERS_CAPACITY = 1024;
const size_t SYMBOLS_CAPACITY = 1024;

Grid parse_input(char *input_path) {
  Coordinates *symbols = malloc(SYMBOLS_CAPACITY * sizeof(Coordinates));
  PositionedNumber *numbers =
      malloc(NUMBERS_CAPACITY * sizeof(PositionedNumber));

  size_t symbols_len = 0;
  size_t numbers_len = 0;

  FileLines file_lines = read_file_lines(input_path);
  for (size_t line_idx = 0; line_idx < file_lines.line_count; ++line_idx) {
    char *initial_str_ptr = file_lines.lines[line_idx];
    char **line_ptr = &file_lines.lines[line_idx];
    while (1) {
      static const size_t number_buffer_capacity = 16;
      char number_buffer[number_buffer_capacity];
      size_t x = (size_t)(*line_ptr - initial_str_ptr);
      maybe_take_natural_number(line_ptr, number_buffer,
                                number_buffer_capacity);
      if (strlen(number_buffer)) {
        Coordinates start_coords = {.x = x, .y = line_idx};
        PositionedNumber positioned_number = {
            .num = atoi(number_buffer),
            .str_len = strlen(number_buffer),
            .start_coords = start_coords,
        };
        if (numbers_len == NUMBERS_CAPACITY - 1) {
          printf("numbers buffer is full\n");
          exit(1);
        }
        numbers[numbers_len++] = positioned_number;
      } else {
        char next_char = (*line_ptr)[0];
        size_t next_char_pos = (size_t)(*line_ptr - initial_str_ptr);
        if (next_char == '\0' || next_char == '\n') {
          break;
        }
        (*line_ptr)++;
        if (next_char != '.') {
          Coordinates symbol = {
              .x = next_char_pos,
              .y = line_idx,
          };
          if (symbols_len == SYMBOLS_CAPACITY - 1) {
            printf("symbols buffer is full\n");
            exit(1);
          }
          symbols[symbols_len++] = symbol;
        }
      }
    }
  }
  return (Grid){.numbers = numbers,
                .numbers_len = numbers_len,
                .symbols = symbols,
                .symbols_len = symbols_len};
}
