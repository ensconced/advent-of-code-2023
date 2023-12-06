#pragma once
#include <stdbool.h>

typedef struct {
  long start;
  long end;
} Interval;

bool contains(Interval container, Interval contained);
Interval intersection(Interval a, Interval b);
bool is_empty(Interval interval);
bool equals(Interval a, Interval b);
