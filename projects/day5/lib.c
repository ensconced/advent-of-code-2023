#include "./lib.h"
#include "../utils/file_utils/lib.h"
#include "../utils/interval/lib.h"
#include "../utils/linked_list/lib.h"
#include "../utils/parser_utils/lib.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const size_t ranges_buffer_capacity = 64;
const size_t range_map_buffer_capacity = 64;
const size_t range_map_numbers_buffer_capacity = 64;
const size_t seeds_buffer_capacity = 128;
const size_t individual_number_buffer_capacity = 16;
const size_t path_buckets_buffer_capacity = 512;
static const size_t mapped_intervals_capacity = 256;

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
  unsigned long dest_range_start = take_number(&line);
  maybe_take_whitespace(&line);
  unsigned long source_range_start = take_number(&line);
  maybe_take_whitespace(&line);
  unsigned long range_len = take_number(&line);
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

bool find_matching_range(RangeMap range_map, unsigned long value,
                         Range *result) {
  for (size_t i = 0; i < range_map.ranges_len; ++i) {
    Range range = range_map.ranges[i];
    if (value >= range.source_range_start &&
        value < range.source_range_start + range.range_len) {
      *result = range;
      return true;
    }
  }
  return false;
}

unsigned long map_seed_to_location(unsigned long seed, Almanac almanac) {
  unsigned long current_value = seed;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    Range matching_range;
    if (find_matching_range(range_map, current_value, &matching_range)) {
      unsigned long offset_within_range =
          current_value - matching_range.source_range_start;
      current_value = matching_range.dest_range_start + offset_within_range;
    }
  }
  return current_value;
}

unsigned long part1_lowest_location(Almanac almanac) {
  unsigned long min = ULONG_MAX;
  for (size_t i = 0; i < almanac.seeds_len; ++i) {
    unsigned long location = map_seed_to_location(almanac.seeds[i], almanac);
    if (location < min) {
      min = location;
    }
  }
  return min;
}

MappedInterval *initial_intervals(Almanac almanac,
                                  size_t *mapped_intervals_len) {
  MappedInterval *mapped_intervals =
      malloc(mapped_intervals_capacity * sizeof(MappedInterval));
  if (mapped_intervals == 0) {
    printf("failed to allocate mapped intervals buffer\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < almanac.seeds_len; i += 2) {
    interval_Interval interval = {
        .start = (long)almanac.seeds[i],
        .end = (long)(almanac.seeds[i] + almanac.seeds[i + 1]),
    };
    MappedInterval initial_interval = {
        .original_interval = interval,
        .current_interval = interval,
    };
    mapped_intervals[(*mapped_intervals_len)++] = initial_interval;
  }
  return mapped_intervals;
}

MappedInterval *apply_range_map(MappedInterval *mapped_intervals,
                                size_t mapped_intervals_len,
                                RangeMap range_map) {

  MappedInterval *new_mapped_intervals =
      malloc(mapped_intervals_capacity * sizeof(MappedInterval));

  for (size_t i = 0; i < mapped_intervals_len; ++i) {
    Range intersecting_range;
    if (find_intersecting_range(mapped_intervals[i], range_map,
                                &intersecting_range)) {
      // TODO - add intersection to new_mapped_intervals
      // TODO - also add difference(original_interval, intersection) to
      // new_mapped_intervals, if not empty
    } else {
      // TODO - add unmodified original_interval to new_mapped_intervals
    }
  }

  free(mapped_intervals);
}

MappedInterval *get_mapped_intervals(Almanac almanac) {
  size_t mapped_intervals_len = 0;
  MappedInterval *mapped_intervals =
      initial_intervals(almanac, &mapped_intervals_len);

  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    MappedInterval *new_mapped_intervals = apply_range_map(
        mapped_intervals, mapped_intervals_len, almanac.range_maps[i]);
    mapped_intervals = new_mapped_intervals;
  }
  return mapped_intervals;
}

// unsigned long part2_lowest_location(Almanac almanac) {
//   MappedInterval *mapped_intervals = get_mapped_intervals(almanac);
//   return 1234;
// }
