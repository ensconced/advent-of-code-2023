#include "./lib.h"
#include "../file_utils/lib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void take_word(char *word, char **str_pointer) {
  size_t word_len = strlen(word);
  if (strncmp(*str_pointer, word, word_len)) {
    printf("expected word \"%s\" to start with \"%s\"\n", *str_pointer, word);
    exit(1);
  }
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

Round *maybe_take_round(char **str_pointer) {
  int red_draw_count = 0;
  int green_draw_count = 0;
  int blue_draw_count = 0;

  int draw_count;
  do {
    draw_count = maybe_take_natural_number(str_pointer);
    if (draw_count) {
      take_word(" ", str_pointer);
      enum Colour colour = take_colour(str_pointer);
      if (colour == RED) {
        red_draw_count = draw_count;
      } else if (colour == GREEN) {
        green_draw_count = draw_count;
      } else if (colour == BLUE) {
        blue_draw_count = draw_count;
      }

      char next_char = (*str_pointer)[0];

      if (next_char == '\0' || next_char == '\n' ||
          maybe_take_word("; ", str_pointer)) {
        break;
      }
      take_word(", ", str_pointer);
    }
  } while (draw_count);

  if (draw_count) {
    Round *round = calloc(1, sizeof(Round));
    if (round == NULL) {
      printf("failed to allocate round\n");
      exit(1);
    }
    round->red = red_draw_count;
    round->green = green_draw_count;
    round->blue = blue_draw_count;
    return round;
  }

  return NULL;
}

Round **take_rounds(size_t *round_count_result, char **str_pointer) {
  size_t rounds_capacity = 256;
  size_t round_count = 0;
  Round **rounds = malloc(rounds_capacity * sizeof(Round *));
  if (rounds == NULL) {
    printf("failed to allocate rounds\n");
    exit(1);
  }
  Round *round;
  do {
    round = maybe_take_round(str_pointer);
    if (round) {
      if (round_count == rounds_capacity) {
        printf("too many rounds\n");
        exit(1);
      }
      rounds[round_count++] = round;
    }

  } while (round != NULL);
  *round_count_result = round_count;
  return rounds;
}

Game parse_line(char *line) {
  char **line_pointer = &line;
  int game_id = take_header_returning_game_id(line_pointer);
  size_t round_count;
  Round **rounds = take_rounds(&round_count, line_pointer);

  return (Game){
      .rounds = rounds,
      .round_count = round_count,
      .id = game_id,
  };
}

int TOTAL_RED_COUNT = 12;
int TOTAL_GREEN_COUNT = 13;
int TOTAL_BLUE_COUNT = 14;

bool game_is_possible(Game game) {
  for (size_t i = 0; i < game.round_count; ++i) {
    Round *round = game.rounds[i];
    if (round->red > TOTAL_RED_COUNT || round->green > TOTAL_GREEN_COUNT ||
        round->blue > TOTAL_BLUE_COUNT)
      return false;
  }
  return true;
}

int sum_possible_game_ids(char *input_path) {
  FileLines game_lines = read_file_lines(input_path);
  int result = 0;
  for (size_t i = 0; i < game_lines.line_count; ++i) {
    Game game = parse_line(game_lines.lines[i]);
    if (game_is_possible(game)) {
      result += game.id;
    }
  }
  return result;
}
