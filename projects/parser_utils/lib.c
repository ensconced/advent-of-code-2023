#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool maybe_take_whitespace(char **str_pointer) {
  size_t taken = 0;
  while (1) {
    char next_char = (*str_pointer)[taken];
    if (next_char == ' ' || next_char == '\t') {
      taken++;
    } else {
      break;
    }
  }
  *str_pointer += taken;
  return !!taken;
}

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

void take_natural_number(char **str_pointer, char *result_buffer,
                         size_t result_buffer_capacity) {
  size_t result_len = 0;
  size_t i;
  for (i = 0;; ++i) {
    char possible_digit = (*str_pointer)[i];
    if (isdigit(possible_digit)) {
      if (i >= result_buffer_capacity - 1) {
        printf("ran out of space in result buffer\n");
        exit(1);
      }
      result_buffer[i] = possible_digit;
      result_len++;
    } else {
      break;
    }
  }
  if (result_len == 0) {
    printf("failed to take natural number\n");
    exit(1);
  }

  result_buffer[i] = '\0';
  (*str_pointer) += result_len;
}

void maybe_take_natural_number(char **str_pointer, char *result_buffer,
                               size_t result_buffer_capacity) {
  char first_digit = (*str_pointer)[0];
  if (isdigit(first_digit) && first_digit != '0') {
    take_natural_number(str_pointer, result_buffer, result_buffer_capacity);
  } else {
    result_buffer[0] = '\0';
  }
}

void take_whitespace_separated_natural_numbers(char **str_pointer,
                                               char **result_buffer,
                                               size_t *result_buffer_len,
                                               size_t result_buffer_capacity,
                                               size_t number_buffer_capacity) {

  *result_buffer_len = 0;
  while (1) {
    char *number_buffer = malloc(number_buffer_capacity);
    if (number_buffer == NULL) {
      printf("failed to allocate number buffer\n");
      exit(1);
    }
    maybe_take_natural_number(str_pointer, number_buffer,
                              number_buffer_capacity);
    maybe_take_whitespace(str_pointer);
    if (strlen(number_buffer)) {
      if ((*result_buffer_len) > result_buffer_capacity - 1) {
        printf("ran out of space in result buffer\n");
        exit(1);
      } else {
        result_buffer[(*result_buffer_len)++] = number_buffer;
      }
    } else {
      free(number_buffer);
      return;
    }
  }
}
