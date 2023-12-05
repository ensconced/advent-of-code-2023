#include "./lib.h"
#include "../file_utils/lib.h"
#include "../parser_utils/lib.h"
#include <stdio.h>
#include <stdlib.h>

const size_t range_map_buffer_capacity = 64;
const size_t seeds_buffer_capacity = 128;
const size_t individual_number_buffer_capacity = 16;

void take_seeds(char **str_pointer, int *seeds_buffer,
                size_t *seeds_buffer_len) {
  take_string("seeds: ", str_pointer);
  take_whitespace_separated_numbers(str_pointer, seeds_buffer, seeds_buffer_len,
                                    seeds_buffer_capacity,
                                    individual_number_buffer_capacity);
}

void take_range_maps(char **str_pointer, RangeMap *range_map_buffer,
                     size_t *range_map_buffer_len,
                     size_t range_map_buffer_capacity) {
  printf("TODO");
  exit(EXIT_FAILURE);
}

Almanac parse_input(char *input_path) {
  char *file_contents = read_text_file(input_path);
  char **str_pointer = &file_contents;

  int *seeds = malloc(seeds_buffer_capacity * sizeof(int));
  size_t seeds_len;
  take_seeds(str_pointer, seeds, &seeds_len);
  take_string("\n\n", str_pointer);

  RangeMap *range_maps = malloc(range_map_buffer_capacity * sizeof(RangeMap));
  size_t range_maps_len;
  take_range_maps(str_pointer, range_maps, &range_maps_len,
                  range_map_buffer_capacity);

  return (Almanac){.seeds = seeds,
                   .seeds_len = seeds_len,
                   .range_maps = range_maps,
                   .range_maps_len = range_maps_len};
}
