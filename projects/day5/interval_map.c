#include "./interval_map.h"
#include "../utils/interval/lib.h"
#include <stdio.h>
#include <stdlib.h>

long interval_map_offset(IntervalMap interval_map) {
  return interval_map.dst_interval.start - interval_map.src_interval.start;
}

IntervalMap interval_map_from_dst_interval(interval_interval dst_interval,
                                           long src_to_dst_offset) {
  return (IntervalMap){
      .src_interval =
          (interval_interval){
              .start = dst_interval.start - src_to_dst_offset,
              .end = dst_interval.end - src_to_dst_offset,
          },
      .dst_interval = dst_interval,
  };
}

interval_map_subtraction_result
interval_map_subtract_dst_interval(IntervalMap interval_map,
                                   interval_interval interval) {
  interval_difference_result dst_remainder =
      interval_difference(interval_map.dst_interval, interval);
  switch (dst_remainder.type) {
  case INTERVAL_PAIR: {
    return (interval_map_subtraction_result){
        .type = INTERVAL_MAP_PAIR,
        .data =
            (interval_map_single_or_pair){
                .pair =
                    {
                        interval_map_from_dst_interval(
                            dst_remainder.data.pair[0],
                            interval_map_offset(interval_map)),
                        interval_map_from_dst_interval(
                            dst_remainder.data.pair[1],
                            interval_map_offset(interval_map)),
                    }},
    };
  }
  case INTERVAL_SINGLE: {
    return (interval_map_subtraction_result){
        .type = INTERVAL_MAP_SINGLE,
        .data =
            (interval_map_single_or_pair){
                .single = interval_map_from_dst_interval(
                    dst_remainder.data.single,
                    interval_map_offset(interval_map)),
            },
    };
  }
  default: {
    printf("unexpected default case\n");
    exit(EXIT_FAILURE);
  }
  }
}

IntervalMap interval_map_merge_intersection(IntervalMap a, IntervalMap b) {
  interval_interval intersection =
      interval_intersection(a.dst_interval, b.src_interval);
  interval_interval src_interval =
      interval_shift(intersection, -interval_map_offset(a));
  interval_interval dst_interval =
      interval_shift(intersection, interval_map_offset(b));
  return (IntervalMap){
      .src_interval = src_interval,
      .dst_interval = dst_interval,
  };
}

void print_interval_map(IntervalMap interval_map) {
  printf("%ld - %ld -> %ld - %ld\n", interval_map.src_interval.start,
         interval_map.src_interval.end, interval_map.dst_interval.start,
         interval_map.dst_interval.end);
}
