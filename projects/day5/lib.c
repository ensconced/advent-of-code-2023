#include "./lib.h"
#include "../utils/interval/lib.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const size_t mapped_intervals_capacity = 1028;

// TODO - can we get rid of this function and use intersection instead?
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

bool find_intersecting_dst_map(IntervalMap interval_map, RangeMap range_map,
                               IntervalMap *result) {
  // bool found = false;
  for (size_t i = 0; i < range_map.interval_maps_len; ++i) {
    IntervalMap range = range_map.interval_maps[i];
    interval_interval intersection =
        interval_intersection(interval_map.dst_interval, range.src_interval);
    if (!interval_is_empty(intersection)) {
      // if (found) {
      //   printf("found multiple intersecting maps for %ld - %ld:\n",
      //          interval_map.dst_interval.start,
      //          interval_map.dst_interval.end);
      //   printf("first map: %ld - %ld\n", result->src_interval.start,
      //          result->src_interval.end);
      //   printf("second map: %ld - %ld\n", range.src_interval.start,
      //          range.src_interval.end);
      //   // exit(EXIT_FAILURE);
      // }
      *result = range;
      return true;
      // found = true;
    }
  }
  return false;
}

void add_new_interval_map(interval_interval new_interval_map_src_interval,
                          long offset, IntervalMap origin_interval_map,
                          IntervalMap *mapped_intervals,
                          size_t *mapped_intervals_len) {
  long connection_offset = new_interval_map_src_interval.start -
                           origin_interval_map.dst_interval.start;
  long new_interval_length =
      new_interval_map_src_interval.end - new_interval_map_src_interval.start;
  long seed_interval_start =
      origin_interval_map.src_interval.start + connection_offset;

  interval_interval src_interval = {
      .start = seed_interval_start,
      .end = seed_interval_start + new_interval_length,
  };

  if (*mapped_intervals_len == mapped_intervals_capacity) {
    printf("mapped intervals buffer at capacity\n");
    exit(EXIT_FAILURE);
  }
  interval_interval shifted_interval =
      interval_shift(new_interval_map_src_interval, offset);
  mapped_intervals[(*mapped_intervals_len)++] = (IntervalMap){
      .src_interval = src_interval,
      .dst_interval = shifted_interval,
  };
}

void split_and_squash(IntervalMap interval_map, RangeMap range_map,
                      IntervalMap *new_interval_maps,
                      size_t *new_interval_maps_len) {
  IntervalMap intersecting_dst_map;
  if (find_intersecting_dst_map(interval_map, range_map,
                                &intersecting_dst_map)) {

    long offset = intersecting_dst_map.dst_interval.start -
                  intersecting_dst_map.src_interval.start;

    interval_interval intersection = interval_intersection(
        interval_map.dst_interval, intersecting_dst_map.src_interval);

    add_new_interval_map(intersection, offset, interval_map, new_interval_maps,
                         new_interval_maps_len);

    interval_difference_result difference =
        interval_difference(interval_map.dst_interval, intersection);

    if (difference.type == INTERVAL_SINGLE) {
      if (!interval_is_empty(difference.data.single)) {

        add_new_interval_map(difference.data.single, 0, interval_map,
                             new_interval_maps, new_interval_maps_len);
      }
    } else if (difference.type == INTERVAL_PAIR) {
      add_new_interval_map(difference.data.pair[0], 0, interval_map,
                           new_interval_maps, new_interval_maps_len);
      add_new_interval_map(difference.data.pair[1], 0, interval_map,
                           new_interval_maps, new_interval_maps_len);
    }
  } else {
    add_new_interval_map(interval_map.dst_interval, 0, interval_map,
                         new_interval_maps, new_interval_maps_len);
  }
}

IntervalMap *apply_interval_maps(IntervalMap *interval_maps,
                                 size_t interval_maps_len, RangeMap range_map,
                                 size_t *len_result) {
  size_t new_interval_maps_len = 0;

  IntervalMap *new_interval_maps =
      malloc(mapped_intervals_capacity * sizeof(IntervalMap));

  for (size_t i = 0; i < interval_maps_len; ++i) {
    IntervalMap interval_map = interval_maps[i];
    split_and_squash(interval_map, range_map, new_interval_maps,
                     &new_interval_maps_len);
  }

  free(interval_maps);
  *len_result = new_interval_maps_len;
  return new_interval_maps;
}

IntervalMap *combine_interval_maps(Almanac almanac,
                                   size_t *mapped_intervals_len_result) {
  size_t mapped_intervals_len = 0;
  IntervalMap *mapped_intervals =
      initial_interval_maps(almanac, &mapped_intervals_len);

  size_t new_interval_maps_len = 0;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    IntervalMap *new_interval_maps =
        apply_interval_maps(mapped_intervals, mapped_intervals_len, range_map,
                            &new_interval_maps_len);
    mapped_intervals = new_interval_maps;
    mapped_intervals_len = new_interval_maps_len;
  }
  *mapped_intervals_len_result = mapped_intervals_len;
  return mapped_intervals;
}

long part2_lowest_location(Almanac almanac) {
  long min = LONG_MAX;
  size_t mapped_intervals_len = 0;
  IntervalMap *mapped_intervals =
      combine_interval_maps(almanac, &mapped_intervals_len);
  for (size_t i = 0; i < mapped_intervals_len; ++i) {
    long interval_start = mapped_intervals[i].dst_interval.start;
    if (interval_start < min) {
      min = interval_start;
    }
  }
  return min;
}
