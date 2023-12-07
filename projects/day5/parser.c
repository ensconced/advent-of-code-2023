#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const size_t seeds_buffer_capacity = 128;
const size_t individual_number_buffer_capacity = 16;
const size_t ranges_buffer_capacity = 64;
const size_t range_map_buffer_capacity = 64;
const size_t range_map_numbers_buffer_capacity = 64;

void take_seeds(FileLines file_lines, size_t *current_line_idx,
                unsigned long *seeds_buffer, size_t *seeds_buffer_len) {
  char *line = file_lines.lines[*current_line_idx];
  char *current_position_within_line = line;
  take_string("seeds: ", &current_position_within_line);
  take_whitespace_separated_numbers(&current_position_within_line, seeds_buffer,
                                    seeds_buffer_len, seeds_buffer_capacity,
                                    individual_number_buffer_capacity);
  (*current_line_idx) += 2;
}

Range take_range(FileLines file_lines, size_t *current_line_idx) {
  char *line = file_lines.lines[(*current_line_idx)++];
  long dest_range_start = (long)take_number(&line);
  maybe_take_whitespace(&line);
  long source_range_start = (long)take_number(&line);
  maybe_take_whitespace(&line);
  long range_len = (long)take_number(&line);
  return (Range){
      .dest_range_start = dest_range_start,
      .source_range_start = source_range_start,
      .range_len = range_len,
  };
}

void take_range_map(FileLines file_lines, size_t *current_line_idx,
                    RangeMap *range_map_buffer, size_t *range_map_buffer_len) {

  Range *ranges_buffer = malloc(ranges_buffer_capacity * sizeof(Range));
  if (ranges_buffer == 0) {
    printf("failed to allocate ranges buffer\n");
    exit(1);
  }
  size_t ranges_buffer_len = 0;

  (*current_line_idx)++;
  while (1) {
    if (*current_line_idx < file_lines.line_count) {
      char *line = file_lines.lines[*current_line_idx];
      if (isdigit(line[0])) {
        Range range = take_range(file_lines, current_line_idx);
        if (ranges_buffer_len == ranges_buffer_capacity) {
          printf("ranges buffer at capacity\n");
          exit(1);
        }
        ranges_buffer[ranges_buffer_len++] = range;
        continue;
      }
    }
    (*current_line_idx)++;
    break;
  }
  if (*range_map_buffer_len == range_map_buffer_capacity) {
    printf("range map buffer full\n");
    exit(EXIT_FAILURE);
  }
  range_map_buffer[(*range_map_buffer_len)++] = (RangeMap){
      .ranges = ranges_buffer,
      .ranges_len = ranges_buffer_len,
  };
}

void take_range_maps(FileLines file_lines, size_t *current_line_idx,
                     RangeMap *range_map_buffer, size_t *range_map_buffer_len) {
  while (*current_line_idx < file_lines.line_count) {
    take_range_map(file_lines, current_line_idx, range_map_buffer,
                   range_map_buffer_len);
  }
}

Almanac parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  size_t current_line_idx = 0;

  unsigned long *seeds = malloc(seeds_buffer_capacity * sizeof(unsigned long));
  size_t seeds_len;
  take_seeds(file_lines, &current_line_idx, seeds, &seeds_len);

  RangeMap *range_maps = malloc(range_map_buffer_capacity * sizeof(RangeMap));
  size_t range_maps_len = 0;
  take_range_maps(file_lines, &current_line_idx, range_maps, &range_maps_len);

  return (Almanac){.seeds = seeds,
                   .seeds_len = seeds_len,
                   .range_maps = range_maps,
                   .range_maps_len = range_maps_len};
}
