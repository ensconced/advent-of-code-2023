#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t cards_per_hand = 5;
const size_t cards_buffer_capacity = cards_per_hand + 1;

int compare_chars(const void *a, const void *b) {
  return *(char *)a - *(char *)b;
}

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

    char *sorted_cards = malloc(cards_buffer_capacity);
    strcpy(sorted_cards, cards);
    qsort(sorted_cards, 5, 1, compare_chars);

    maybe_take_whitespace(&current_position_in_line);
    int bid = (int)take_number(&current_position_in_line);
    hands[i] = (Hand){
        .cards = cards,
        .sorted_cards = sorted_cards,
        .bid = bid,
    };
  }

  return (Hands){
      .hands = hands,
      .hands_len = file_lines.line_count,
  };
}
