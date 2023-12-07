#pragma once
#include "../utils/interval/lib.h"
#include <stddef.h>

typedef struct {
  long dest_range_start;
  long source_range_start;
  long range_len;
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
  interval_interval seed_interval;
  interval_interval current_interval;
} MappedInterval;

long part1_lowest_location(Almanac almanac);
long part2_lowest_location(Almanac almanac);
