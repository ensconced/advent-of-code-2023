#include "./lib.h"
#include "../utils/interval/lib.h"
#include "./interval_map.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const size_t mapped_intervals_capacity = 1028;

bool find_matching_interval_map(RangeMap range_map, long value,
                                IntervalMap *result) {
  for (size_t i = 0; i < range_map.interval_maps_len; ++i) {
    IntervalMap range = range_map.interval_maps[i];
    if (value >= range.src_interval.start && value < range.src_interval.end) {
      *result = range;
      return true;
    }
  }
  return false;
}

long map_seed_to_location(long seed, Almanac almanac) {
  long current_value = seed;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    IntervalMap matching_interval_map;
    if (find_matching_interval_map(range_map, current_value,
                                   &matching_interval_map)) {
      long offset_within_range =
          current_value - matching_interval_map.src_interval.start;
      current_value =
          matching_interval_map.dst_interval.start + offset_within_range;
    }
  }
  return current_value;
}

long part1_lowest_location(Almanac almanac) {
  long min = LONG_MAX;
  for (size_t i = 0; i < almanac.seeds_len; ++i) {
    long location = map_seed_to_location((long)almanac.seeds[i], almanac);
    if (location < min) {
      min = location;
    }
  }
  return min;
}

IntervalMap *initial_interval_maps(Almanac almanac,
                                   size_t *mapped_intervals_len) {
  IntervalMap *mapped_intervals =
      malloc(mapped_intervals_capacity * sizeof(IntervalMap));
  if (mapped_intervals == 0) {
    printf("failed to allocate mapped intervals buffer\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < almanac.seeds_len; i += 2) {
    interval_interval interval = {
        .start = (long)almanac.seeds[i],
        .end = (long)(almanac.seeds[i] + almanac.seeds[i + 1]),
    };
    IntervalMap initial_interval = {
        .src_interval = interval,
        .dst_interval = interval,
    };
    mapped_intervals[(*mapped_intervals_len)++] = initial_interval;
  }
  return mapped_intervals;
}

// TODO - we're checking intersection here but then sorta doing it again in
// interval_map_merge_intersection?
bool find_intersecting_interval_map(IntervalMap interval_map,
                                    RangeMap range_map, IntervalMap *result) {
  for (size_t i = 0; i < range_map.interval_maps_len; ++i) {
    IntervalMap intersector = range_map.interval_maps[i];
    if (!interval_is_empty(interval_intersection(interval_map.dst_interval,
                                                 intersector.src_interval))) {
      *result = intersector;
      return true;
    }
  }
  return false;
}

IntervalMap *get_next_interval_maps(IntervalMap *interval_maps,
                                    size_t interval_maps_len,
                                    RangeMap range_map, size_t *len_result) {
  size_t new_interval_maps_len = 0;

  IntervalMap *new_interval_maps =
      malloc(mapped_intervals_capacity * sizeof(IntervalMap));

  for (size_t i = 0; i < interval_maps_len; ++i) {
    IntervalMap interval_map = interval_maps[i];
    IntervalMap intersecting_interval_map;
    if (find_intersecting_interval_map(interval_map, range_map,
                                       &intersecting_interval_map)) {
      IntervalMap merged = interval_map_merge_intersection(
          interval_map, intersecting_interval_map);
      new_interval_maps[new_interval_maps_len++] = merged;

      interval_map_subtraction_result remainder =
          interval_map_subtract_dst_interval(
              interval_map, intersecting_interval_map.src_interval);

      if (remainder.type == INTERVAL_MAP_SINGLE) {
        if (!interval_is_empty(remainder.data.single.src_interval)) {
          interval_maps[interval_maps_len++] = remainder.data.single;
        }
      } else if (remainder.type == INTERVAL_MAP_PAIR) {
        interval_maps[interval_maps_len++] = remainder.data.pair[0];
        interval_maps[interval_maps_len++] = remainder.data.pair[1];
      }
    } else {
      new_interval_maps[new_interval_maps_len++] = interval_map;
    }
  }

  free(interval_maps);
  *len_result = new_interval_maps_len;
  return new_interval_maps;
}

void print_interval_maps(IntervalMap *interval_maps, size_t interval_maps_len) {
  for (size_t i = 0; i < interval_maps_len; ++i) {
    print_interval_map(interval_maps[i]);
  }
  printf("\n");
}

IntervalMap *get_interval_maps(Almanac almanac,
                               size_t *interval_maps_len_result) {
  size_t interval_maps_len = 0;
  IntervalMap *interval_maps =
      initial_interval_maps(almanac, &interval_maps_len);

  size_t next_interval_maps_len = 0;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    IntervalMap *next_interval_maps = get_next_interval_maps(
        interval_maps, interval_maps_len, range_map, &next_interval_maps_len);
    interval_maps = next_interval_maps;
    interval_maps_len = next_interval_maps_len;
  }
  *interval_maps_len_result = interval_maps_len;
  return interval_maps;
}

long part2_lowest_location(Almanac almanac) {
  long min = LONG_MAX;
  size_t mapped_intervals_len = 0;
  IntervalMap *mapped_intervals =
      get_interval_maps(almanac, &mapped_intervals_len);
  for (size_t i = 0; i < mapped_intervals_len; ++i) {
    long interval_start = mapped_intervals[i].dst_interval.start;
    if (interval_start < min) {
      min = interval_start;
    }
  }
  return min;
}
