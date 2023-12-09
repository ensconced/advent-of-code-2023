#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_chars(const void *a, const void *b) {
  return *(char *)a - *(char *)b;
}

Hands parse_input(char *input_path, bool part2) {
  FileLines file_lines = read_file_lines(input_path);
  Hand *hands = malloc(file_lines.line_count * sizeof(Hand));
  if (hands == 0) {
    printf("failed to allocate hands\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < file_lines.line_count; ++i) {
    char *line = file_lines.lines[i];
    char *current_position_in_line = line;

    char *cards = malloc(cards_in_a_hand + 1);
    take_alphanumeric_string(&current_position_in_line, cards,
                             cards_in_a_hand + 1);

    char *sorted_cards = malloc(cards_in_a_hand + 1);
    strcpy(sorted_cards, cards);
    if (part2) {
      char *with_jokers_replaced = replace_jokers(cards);
      free(sorted_cards);
      sorted_cards = with_jokers_replaced;
    }
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
