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

// return value of zero means no natural number was found
int maybe_take_natural_number(char **str_pointer) {
  char first_digit = (*str_pointer)[0];
  if (isdigit(first_digit) && first_digit != '0') {
    return take_natural_number(str_pointer);
  }
  return 0;
}
