#include "./lib.h"
#include "../file_utils/lib.h"
#include "../parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

static size_t individual_number_buffer_capacity = 16;
static size_t number_buffers_capacity = 256;

Card parse_line(char *line) {
  char **line_ptr = &line;
  take_string("Card ", line_ptr);
  maybe_take_whitespace(line_ptr);
  static const size_t number_buffer_capacity = 16;
  char number_buffer[number_buffer_capacity];
  take_numeric_string(line_ptr, number_buffer, number_buffer_capacity);
  int card_idx = atoi(number_buffer);
  take_string(":", line_ptr);
  maybe_take_whitespace(line_ptr);

  char **winning_numbers_buffer =
      malloc(number_buffers_capacity * sizeof(char *));
  if (winning_numbers_buffer == 0) {
    printf("failed to allocate winning numbers buffer\n");
    exit(EXIT_FAILURE);
  }
  size_t winning_numbers_len;
  take_whitespace_separated_numeric_strings(
      line_ptr, winning_numbers_buffer, &winning_numbers_len,
      number_buffers_capacity, individual_number_buffer_capacity);

  take_string("|", line_ptr);
  maybe_take_whitespace(line_ptr);

  char **my_numbers_buffer = malloc(number_buffers_capacity * sizeof(char *));
  if (my_numbers_buffer == 0) {
    printf("failed to allocate my numbers buffer\n");
    exit(EXIT_FAILURE);
  }
  size_t my_numbers_len;
  take_whitespace_separated_numeric_strings(
      line_ptr, my_numbers_buffer, &my_numbers_len, number_buffers_capacity,
      individual_number_buffer_capacity);

  int *my_numbers = malloc(my_numbers_len * sizeof(int));
  for (size_t i = 0; i < my_numbers_len; ++i) {
    my_numbers[i] = atoi(my_numbers_buffer[i]);
  }
  free(my_numbers_buffer);

  int *winning_numbers = malloc(winning_numbers_len * sizeof(int));
  for (size_t i = 0; i < winning_numbers_len; ++i) {
    winning_numbers[i] = atoi(winning_numbers_buffer[i]);
  }
  free(winning_numbers_buffer);

  return (Card){
      .count = 1,
      .idx = card_idx,
      .winning_numbers = winning_numbers,
      .winning_numbers_len = winning_numbers_len,
      .my_numbers = my_numbers,
      .my_numbers_len = my_numbers_len,
  };
}

int part1_card_score(Card card) {
  int score = 0;
  for (size_t i = 0; i < card.my_numbers_len; ++i) {
    for (size_t j = 0; j < card.winning_numbers_len; ++j) {
      if (card.my_numbers[i] == card.winning_numbers[j]) {
        score = score == 0 ? 1 : score * 2;
        break;
      }
    }
  }
  return score;
}

int part2_card_score(Card card) {
  int score = 0;
  for (size_t i = 0; i < card.my_numbers_len; ++i) {
    for (size_t j = 0; j < card.winning_numbers_len; ++j) {
      if (card.my_numbers[i] == card.winning_numbers[j]) {
        score++;
        break;
      }
    }
  }
  return score;
}

int part1(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  int sum = 0;
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    Card card = parse_line(file_lines.lines[i]);
    sum += part1_card_score(card);
  }
  free_file_lines(file_lines);
  return sum;
}

int part2(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  Card *cards = malloc(file_lines.line_count * sizeof(Card));
  if (cards == 0) {
    printf("failed to allocate cards buffer\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    cards[i] = parse_line(file_lines.lines[i]);
  }
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    Card card = cards[i];
    int score = part2_card_score(card);
    size_t duplication_end_idx_incl =
        MIN(i + (size_t)score, file_lines.line_count - 1);
    for (size_t j = i + 1; j <= duplication_end_idx_incl; ++j) {
      cards[j].count += card.count;
    }
  }
  int result = 0;
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    Card card = cards[i];
    result += card.count;
  }
  free_file_lines(file_lines);
  return result;
}
