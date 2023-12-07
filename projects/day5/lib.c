#include "./lib.h"
#include "../utils/interval/lib.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const size_t mapped_intervals_capacity = 1028;

bool find_matching_range(RangeMap range_map, long value, Range *result) {
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

long map_seed_to_location(long seed, Almanac almanac) {
  long current_value = seed;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    Range matching_range;
    if (find_matching_range(range_map, current_value, &matching_range)) {
      long offset_within_range =
          current_value - matching_range.source_range_start;
      current_value = matching_range.dest_range_start + offset_within_range;
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

MappedInterval *initial_intervals(Almanac almanac,
                                  size_t *mapped_intervals_len) {
  MappedInterval *mapped_intervals =
      malloc(mapped_intervals_capacity * sizeof(MappedInterval));
  if (mapped_intervals == 0) {
    printf("failed to allocate mapped intervals buffer\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < almanac.seeds_len; i += 2) {
    interval_interval interval = {
        .start = (long)almanac.seeds[i],
        .end = (long)(almanac.seeds[i] + almanac.seeds[i + 1]),
    };
    MappedInterval initial_interval = {
        .seed_interval = interval,
        .current_interval = interval,
    };
    mapped_intervals[(*mapped_intervals_len)++] = initial_interval;
  }
  return mapped_intervals;
}

bool find_intersecting_range(MappedInterval mapped_interval, RangeMap range_map,
                             Range *result) {
  for (size_t i = 0; i < range_map.ranges_len; ++i) {
    Range range = range_map.ranges[i];
    // TODO - it would be easier if we represented the ranges using intervals
    // from the start (i.e. while parsing)
    interval_interval range_interval = {
        .start = (long)range.source_range_start,
        .end = (long)(range.source_range_start + range.range_len),
    };
    if (!interval_is_empty(interval_intersection(
            mapped_interval.current_interval, range_interval))) {
      *result = range;
      return true;
    }
  }
  return false;
}

void add_new_interval(interval_interval current_interval, long offset,
                      MappedInterval mapped_interval,
                      MappedInterval *mapped_intervals,
                      size_t *mapped_intervals_len) {
  long new_interval_offset =
      current_interval.start - mapped_interval.current_interval.start;
  long new_interval_length = current_interval.end - current_interval.start;
  long seed_interval_start =
      mapped_interval.seed_interval.start + new_interval_offset;

  interval_interval seed_interval = {
      .start = seed_interval_start,
      .end = seed_interval_start + new_interval_length,
  };

  if (*mapped_intervals_len == mapped_intervals_capacity) {
    printf("mapped intervals buffer at capacity\n");
    exit(EXIT_FAILURE);
  }
  interval_interval shifted_interval = interval_shift(current_interval, offset);
  mapped_intervals[(*mapped_intervals_len)++] = (MappedInterval){
      .seed_interval = seed_interval,
      .current_interval = shifted_interval,
  };
}

MappedInterval *apply_range_map(MappedInterval *mapped_intervals,
                                size_t mapped_intervals_len, RangeMap range_map,
                                size_t *len_result) {
  size_t new_mapped_intervals_len = 0;

  MappedInterval *new_mapped_intervals =
      malloc(mapped_intervals_capacity * sizeof(MappedInterval));

  for (size_t i = 0; i < mapped_intervals_len; ++i) {
    MappedInterval mapped_interval = mapped_intervals[i];
    Range intersecting_range;
    if (find_intersecting_range(mapped_interval, range_map,
                                &intersecting_range)) {
      interval_interval range_interval = {
          .start = (long)intersecting_range.source_range_start,
          .end = (long)(intersecting_range.source_range_start +
                        intersecting_range.range_len),
      };

      long range_offset = intersecting_range.dest_range_start -
                          intersecting_range.source_range_start;

      interval_interval intersection = interval_intersection(
          mapped_interval.current_interval, range_interval);

      add_new_interval(intersection, range_offset, mapped_interval,
                       new_mapped_intervals, &new_mapped_intervals_len);

      interval_difference_result difference =
          interval_difference(mapped_interval.current_interval, intersection);

      if (difference.type == INTERVAL_SINGLE) {
        if (!interval_is_empty(difference.data.single)) {
          add_new_interval(difference.data.single, 0, mapped_interval,
                           new_mapped_intervals, &new_mapped_intervals_len);
        }
      } else if (difference.type == INTERVAL_PAIR) {
        add_new_interval(difference.data.pair[0], 0, mapped_interval,
                         new_mapped_intervals, &new_mapped_intervals_len);
        add_new_interval(difference.data.pair[1], 0, mapped_interval,
                         new_mapped_intervals, &new_mapped_intervals_len);
      }
    } else {
      add_new_interval(mapped_interval.current_interval, 0, mapped_interval,
                       new_mapped_intervals, &new_mapped_intervals_len);
    }
  }

  free(mapped_intervals);
  *len_result = new_mapped_intervals_len;
  return new_mapped_intervals;
}

MappedInterval *get_mapped_intervals(Almanac almanac,
                                     size_t *mapped_intervals_len_result) {
  size_t mapped_intervals_len = 0;
  MappedInterval *mapped_intervals =
      initial_intervals(almanac, &mapped_intervals_len);

  size_t new_mapped_intervals_len = 0;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    MappedInterval *new_mapped_intervals =
        apply_range_map(mapped_intervals, mapped_intervals_len, range_map,
                        &new_mapped_intervals_len);
    mapped_intervals = new_mapped_intervals;
    mapped_intervals_len = new_mapped_intervals_len;
  }
  *mapped_intervals_len_result = mapped_intervals_len;
  return mapped_intervals;
}

long part2_lowest_location(Almanac almanac) {
  long min = LONG_MAX;
  size_t mapped_intervals_len = 0;
  MappedInterval *mapped_intervals =
      get_mapped_intervals(almanac, &mapped_intervals_len);
  for (size_t i = 0; i < mapped_intervals_len; ++i) {
    long interval_start = mapped_intervals[i].current_interval.start;
    if (interval_start < min) {
      min = interval_start;
    }
  }
  return min;
}
