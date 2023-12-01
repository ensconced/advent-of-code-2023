#include "../file_utils/lib.h"
#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *spelled_digits[] = {"zero", "one", "two",   "three", "four",
                          "five", "six", "seven", "eight", "nine"};

char spelled_digit_at_index(char *str, size_t index) {
  for (size_t i = 0; i < 10; ++i) {
    char *spelled_digit = spelled_digits[i];
    size_t spelled_digit_len = strlen(spelled_digit);
    if (strncmp(str + index, spelled_digit, spelled_digit_len) == 0) {
      return '0' + (char)i;
    }
  }
  return '\0';
}

char get_digit(char *str, size_t index, bool part2) {
  if (isdigit(str[index])) {
    return str[index];
  } else if (part2) {
    return spelled_digit_at_index(str, index);
  }
  return '\0';
}

int line_value(char *line, bool part2) {
  size_t len = strlen(line);
  char first_val = '\0';
  char last_val = '\0';
  for (size_t i = 0; i < len; ++i) {
    char digit = get_digit(line, i, part2);
    if (digit) {
      if (first_val == '\0') {
        first_val = digit;
      }
      last_val = digit;
    }
  }
  char str[3] = {first_val, last_val, '\0'};
  return atoi(str);
}

int sum_calibration_values(char *input_path, bool part2) {
  FileLines file_lines = read_file_lines(input_path);
  int sum = 0;
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    sum += line_value(file_lines.lines[i], part2);
  }
  return sum;
}
