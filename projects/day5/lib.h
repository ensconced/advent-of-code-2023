#pragma once
#include "../utils/interval/lib.h"
#include "./interval_map.h"
#include <stddef.h>

typedef struct {
  IntervalMap *interval_maps;
  size_t interval_maps_len;
} RangeMap;

typedef struct {
  unsigned long *seeds;
  size_t seeds_len;
  RangeMap *range_maps;
  size_t range_maps_len;
} Almanac;

long part1_lowest_location(Almanac almanac);
long part2_lowest_location(Almanac almanac);
