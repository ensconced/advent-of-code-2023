#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

char **parse_numbers(char **str_pointer, size_t *result_len) {
  static const size_t number_buffer_capacity = 16;
  static const size_t result_buffer_capacity = 128;

  char **result_buffer = malloc(result_buffer_capacity * sizeof(char *));

  if (result_buffer == 0) {
    printf("failed to allocate result buffer\n");
    exit(EXIT_FAILURE);
  }
  take_whitespace_separated_numeric_strings(str_pointer, result_buffer,
                                            result_len, result_buffer_capacity,
                                            number_buffer_capacity);
  return result_buffer;
}

Races parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  char *times_str = file_lines.lines[0];
  char *times_str_pointer = times_str;
  take_string("Time:", &times_str_pointer);
  maybe_take_whitespace(&times_str_pointer);
  size_t times_len = 0;
  char **times = parse_numbers(&times_str_pointer, &times_len);

  char *records_str = file_lines.lines[1];
  char *records_str_pointer = records_str;
  take_string("Distance:", &records_str_pointer);
  maybe_take_whitespace(&records_str_pointer);
  size_t records_len = 0;
  char **records = parse_numbers(&records_str_pointer, &records_len);

  assert(times_len == records_len);

  return (Races){
      .times = times,
      .records = records,
      .len = times_len,
  };
}
