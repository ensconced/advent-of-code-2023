#include "../file_utils/lib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_value(char *line) {
  size_t len = strlen(line);
  char first_val = '\0';
  char last_val = '\0';
  for (size_t i = 0; i < len; ++i) {
    if (isdigit(line[i])) {
      if (first_val == '\0') {
        first_val = line[i];
      }
      last_val = line[i];
    }
  }
  char str[3] = {first_val, last_val, '\0'};
  return atoi(str);
}

int sum_calibration_values(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  int sum = 0;
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    sum += line_value(file_lines.lines[i]);
  }
  return sum;
}
