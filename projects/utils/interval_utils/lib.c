#include "./lib.h"
#include <stdbool.h>

bool is_empty(Interval interval) { return interval.end <= interval.start; }

bool equals(Interval a, Interval b) {
  return a.start == b.start && a.end == b.end;
}

bool contains(Interval container, Interval contained) {
  return container.start <= contained.start && container.end >= contained.end;
}

Interval intersection(Interval a, Interval b) {
  if (contains(a, b))
    return b;
  if (contains(b, a))
    return a;
  if (b.start >= a.start && b.start <= a.end)
    return (Interval){
        .start = b.start,
        .end = a.end,
    };
  if (a.start >= b.start && a.start <= b.end)
    return (Interval){
        .start = a.start,
        .end = b.end,
    };
  return (Interval){
      .start = 0,
      .end = 0,
  };
}
