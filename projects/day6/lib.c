#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_ways_of_winning(unsigned long time, unsigned long record) {
  size_t ways_of_winning = 0;
  for (unsigned long j = 0; j <= time; ++j) {
    unsigned long distance = j * (time - j);
    if (distance > record) {
      ways_of_winning++;
    }
  }
  return ways_of_winning;
}

unsigned long part1(char *input_path) {
  Races races = parse_input(input_path);
  unsigned long result = 1;
  for (size_t i = 0; i < races.len; ++i) {
    unsigned long time = strtoul(races.times[i], 0, 10);
    unsigned long record = strtoul(races.records[i], 0, 10);
    result *= count_ways_of_winning(time, record);
  }
  return result;
}

char *concat_strs(char **strs, size_t strs_len) {
  static const size_t result_buffer_capacity = 128;
  char *result_buffer = malloc(result_buffer_capacity);
  if (result_buffer == 0) {
    printf("failed to allocate result buffer\n");
    exit(EXIT_FAILURE);
  }
  result_buffer[0] = '\0';
  for (size_t i = 0; i < strs_len; ++i) {
    size_t len_after_cat = strlen(result_buffer) + strlen(strs[i]) + 1;
    if (len_after_cat > result_buffer_capacity) {
      printf("ran out of result buffer capacity\n");
      exit(EXIT_FAILURE);
    }
    strcat(result_buffer, strs[i]);
  }
  return result_buffer;
}

unsigned long part2(char *input_path) {
  Races races = parse_input(input_path);
  unsigned long time = strtoul(concat_strs(races.times, races.len), 0, 10);
  unsigned long record = strtoul(concat_strs(races.records, races.len), 0, 10);
  return count_ways_of_winning(time, record);
}
