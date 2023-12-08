#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>

const size_t cards_per_hand = 5;
const size_t cards_buffer_capacity = cards_per_hand + 1;

Hands parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  Hand *hands = malloc(file_lines.line_count * sizeof(Hand));
  if (hands == 0) {
    printf("failed to allocate hands\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < file_lines.line_count; ++i) {
    char *line = file_lines.lines[i];
    char *current_position_in_line = line;
    char *cards = malloc(cards_buffer_capacity);
    take_alphanumeric_string(&current_position_in_line, cards,
                             cards_buffer_capacity);
    maybe_take_whitespace(&current_position_in_line);
    int bid = (int)take_number(&current_position_in_line);
    hands[i] = (Hand){
        .cards = cards,
        .bid = bid,
    };
  }

  return (Hands){
      .hands = hands,
      .hands_len = file_lines.line_count,
  };
}
