#include "./lib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct Game {
//   Round **rounds;
//   size_t round_count;
//   int id;
// } Game;

// typedef struct Round {
//   int red;
//   int blue;
//   int green;
// } Round;

void take_word(char *word, char **str_pointer) {
  size_t word_len = strlen(word);
  assert(strncmp(*str_pointer, word, word_len) == 0);
  *str_pointer += word_len;
}

bool maybe_take_word(char *word, char **str_pointer) {
  size_t word_len = strlen(word);
  if (strncmp(*str_pointer, word, word_len) == 0) {
    *str_pointer += word_len;
    return true;
  }
  return false;
}

int take_natural_number(char **str_pointer) {
  size_t max_digits = 15;
  char buffer[16];
  size_t i;
  for (i = 0; i < max_digits; ++i) {
    char possible_digit = (*str_pointer)[0];
    if (isdigit(possible_digit)) {
      buffer[i] = possible_digit;
      (*str_pointer)++;
    } else {
      break;
    }
  }
  buffer[i] = '\0';
  return atoi(buffer);
}

int take_header_returning_game_id(char **str_pointer) {
  take_word("Game ", str_pointer);
  int game_id = take_natural_number(str_pointer);
  take_word(": ", str_pointer);
  return game_id;
}

// return value of zero means no natural number was found
int maybe_take_natural_number(char **str_pointer) {
  char first_digit = (*str_pointer)[0];
  if (isdigit(first_digit) && first_digit != '0') {
    return take_natural_number(str_pointer);
  }
  return 0;
}

enum Colour take_colour(char **str_pointer) {
  if (maybe_take_word("red", str_pointer))
    return RED;
  if (maybe_take_word("blue", str_pointer))
    return BLUE;
  if (maybe_take_word("green", str_pointer))
    return GREEN;
  printf("expected colour\n");
  exit(1);
}

// 3 blue, 4 red

// Round *maybe_take_round(char **str_pointer) {
//   Round *round = calloc(1, sizeof(Round));

//   int draw_count;
//   do {
//     draw_count = maybe_take_natural_number(str_pointer);
//     if (draw_count) {
//       take_word(" ", str_pointer);
//       enum Colour colour = take_colour();
//       if (colour == RED) {
//         round->red = draw_count;
//       } else if (colour == GREEN) {
//         round->green = draw_count;
//       } else if (colour == BLUE) {
//         round->blue = draw_count;
//       }

//       if (maybe_take_end_of_round()) {
//         break;
//       }
//     }
//   } while (draw_count);

//   return round;
// }

// Round **take_rounds(size_t *round_count_result) {
//   size_t rounds_capacity = 256;
//   size_t round_count = 0;
//   Round **rounds = malloc(rounds_capacity * sizeof(Round *));
//   if (rounds == NULL) {
//     println("failed to allocate rounds\n");
//     exit(1);
//   }
//   Round *round;
//   do {
//     round = maybe_take_round();
//     if (round) {
//       if (round_count == rounds_capacity) {
//         println("too many rounds\n");
//         exit(1);
//       }
//       rounds[round_count++] = round;
//     }

//   } while (round == NULL);
//   *round_count_result = round_count;
//   return rounds;
// }

// Game parse_line(char *line, size_t line_index) {
//   size_t game_id = take_header_returning_game_id(&line);
//   size_t round_count;
//   Round **rounds = take_rounds(&round_count);

//   return (Game){
//       .rounds = rounds,
//       .round_count = round_count,
//       .id = line_index,
//   };
// }
