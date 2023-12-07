#pragma once
#include <stdbool.h>

typedef struct {
  long start;
  long end;
} interval_interval;

typedef enum {
  INTERVAL_SINGLE,
  INTERVAL_PAIR,
} interval_difference_result_type;

typedef union {
  interval_interval single;
  interval_interval pair[2];
} interval_single_or_pair;

typedef struct {
  interval_difference_result_type type;
  interval_single_or_pair data;
} interval_difference_result;

bool interval_is_empty(interval_interval interval);
bool interval_equal(interval_interval a, interval_interval b);
bool interval_contains(interval_interval container,
                       interval_interval contained);
interval_interval interval_intersection(interval_interval a,
                                        interval_interval b);
interval_difference_result interval_difference(interval_interval a,
                                               interval_interval b);

interval_interval interval_shift(interval_interval interval, long offset);
