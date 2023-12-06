#pragma once
#include "../utils/interval_utils/lib.h"
#include <stddef.h>

typedef struct {
  unsigned long dest_range_start;
  unsigned long source_range_start;
  unsigned long range_len;
} Range;

typedef struct {
  Range *ranges;
  size_t ranges_len;
} RangeMap;

typedef struct {
  unsigned long *seeds;
  size_t seeds_len;
  RangeMap *range_maps;
  size_t range_maps_len;
} Almanac;

typedef struct {
  Interval original_interval;
  Interval current_interval;
} MappedInterval;

Almanac parse_input(char *input_path);
unsigned long part1_lowest_location(Almanac almanac);
unsigned long part2_lowest_location(Almanac almanac);
