#pragma once
#include "../utils/interval/lib.h"

typedef struct {
  interval_interval src_interval;
  interval_interval dst_interval;
} IntervalMap;

typedef enum {
  INTERVAL_MAP_SINGLE,
  INTERVAL_MAP_PAIR,
} interval_map_subtraction_result_type;

typedef union {
  IntervalMap single;
  IntervalMap pair[2];
} interval_map_single_or_pair;

typedef struct {
  interval_map_subtraction_result_type type;
  interval_map_single_or_pair data;
} interval_map_subtraction_result;

interval_map_subtraction_result
interval_map_subtract_dst_interval(IntervalMap interval_map,
                                   interval_interval interval);

IntervalMap interval_map_merge_intersection(IntervalMap a, IntervalMap b);
void print_interval_map(IntervalMap interval_map);
