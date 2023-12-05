#include "./lib.h"
#include "../file_utils/lib.h"
#include "../parser_utils/lib.h"
#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int take_header_returning_game_id(char **str_pointer) {
  take_string("Game ", str_pointer);
  static const size_t number_buffer_capacity = 16;
  char number_buffer[number_buffer_capacity];
  take_numeric_string(str_pointer, number_buffer, 16);
  int game_id = atoi(number_buffer);
  take_string(": ", str_pointer);
  return game_id;
}

enum Colour take_colour(char **str_pointer) {
  if (maybe_take_string("red", str_pointer))
    return RED;
  if (maybe_take_string("blue", str_pointer))
    return BLUE;
  if (maybe_take_string("green", str_pointer))
    return GREEN;
  printf("expected colour\n");
  exit(EXIT_FAILURE);
}

Round *maybe_take_round(char **str_pointer) {
  int red_draw_count = 0;
  int green_draw_count = 0;
  int blue_draw_count = 0;

  int draw_count;
  do {
    static const size_t num_buffer_capacity = 16;
    char number_buffer[num_buffer_capacity];
    maybe_take_numeric_string(str_pointer, number_buffer, num_buffer_capacity);
    draw_count = atoi(number_buffer);

    if (draw_count) {
      take_string(" ", str_pointer);
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
          maybe_take_string("; ", str_pointer)) {
        break;
      }
      take_string(", ", str_pointer);
    }
  } while (draw_count);

  if (draw_count) {
    Round *round = malloc(sizeof(Round));
    if (round == 0) {
      printf("failed to allocate round\n");
      exit(EXIT_FAILURE);
    }
    round->red = red_draw_count;
    round->green = green_draw_count;
    round->blue = blue_draw_count;
    return round;
  }

  return 0;
}

Round **take_rounds(size_t *round_count_result, char **str_pointer) {
  size_t rounds_capacity = 256;
  size_t round_count = 0;
  Round **rounds = malloc(rounds_capacity * sizeof(Round *));
  if (rounds == 0) {
    printf("failed to allocate rounds\n");
    exit(EXIT_FAILURE);
  }
  Round *round;
  do {
    round = maybe_take_round(str_pointer);
    if (round) {
      if (round_count == rounds_capacity) {
        printf("too many rounds\n");
        exit(EXIT_FAILURE);
      }
      rounds[round_count++] = round;
    }

  } while (round != 0);
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

int game_power(Game game) {
  int min_red = 0;
  int min_green = 0;
  int min_blue = 0;
  for (size_t i = 0; i < game.round_count; ++i) {
    Round *round = game.rounds[i];
    if (round->red > min_red)
      min_red = round->red;
    if (round->green > min_green)
      min_green = round->green;
    if (round->blue > min_blue)
      min_blue = round->blue;
  }
  return min_red * min_green * min_blue;
}

int sum_possible_game_ids(char *input_path) {
  FileLines game_lines = read_file_lines(input_path);
  int result = 0;
  for (size_t i = 0; i < game_lines.line_count; ++i) {
    Game game = parse_line(game_lines.lines[i]);
    if (game_is_possible(game)) {
      result += game.id;
    }
    free_game(game);
  }
  free_file_lines(game_lines);
  return result;
}

int sum_game_powers(char *input_path) {
  FileLines game_lines = read_file_lines(input_path);
  int result = 0;
  for (size_t i = 0; i < game_lines.line_count; ++i) {
    Game game = parse_line(game_lines.lines[i]);
    result += game_power(game);
    free_game(game);
  }
  free_file_lines(game_lines);
  return result;
}

void free_game(Game game) {
  for (size_t i = 0; i < game.round_count; ++i) {
    free(game.rounds[i]);
  }
  free(game.rounds);
}
