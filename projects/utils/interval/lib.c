#include "./lib.h"
#include <stdbool.h>

bool interval_is_empty(interval_interval interval) {
  return interval.end <= interval.start;
}

bool interval_equal(interval_interval a, interval_interval b) {
  return a.start == b.start && a.end == b.end;
}

bool interval_contains(interval_interval container,
                       interval_interval contained) {
  return container.start <= contained.start && container.end >= contained.end;
}

interval_interval interval_intersection(interval_interval a,
                                        interval_interval b) {
  if (interval_contains(a, b))
    return b;
  if (interval_contains(b, a))
    return a;
  if (b.start >= a.start && b.start <= a.end)
    return (interval_interval){
        .start = b.start,
        .end = a.end,
    };
  if (a.start >= b.start && a.start <= b.end)
    return (interval_interval){
        .start = a.start,
        .end = b.end,
    };
  return (interval_interval){
      .start = 0,
      .end = 0,
  };
}

interval_difference_result interval_difference(interval_interval a,
                                               interval_interval b) {

  interval_interval intersection = interval_intersection(a, b);
  if (interval_is_empty(intersection)) {
    return (interval_difference_result){
        .type = INTERVAL_SINGLE,
        .data = (interval_single_or_pair){.single = a},
    };
  }
  if (intersection.start == a.start) {
    return (interval_difference_result){
        .type = INTERVAL_SINGLE,
        .data =
            (interval_single_or_pair){
                .single =
                    (interval_interval){
                        .start = intersection.end,
                        .end = a.end,
                    },
            },
    };
  }
  if (intersection.end == a.end) {
    return (interval_difference_result){
        .type = INTERVAL_SINGLE,
        .data =
            (interval_single_or_pair){
                .single =
                    (interval_interval){
                        .start = a.start,
                        .end = intersection.start,
                    },
            },
    };
  }
  return (interval_difference_result){
      .type = INTERVAL_PAIR,
      .data =
          (interval_single_or_pair){
              .pair =
                  {
                      (interval_interval){
                          .start = a.start,
                          .end = b.start,
                      },
                      (interval_interval){
                          .start = b.end,
                          .end = a.end,
                      },
                  },
          },
  };
}

interval_interval interval_shift(interval_interval interval, long offset) {
  return (interval_interval){.start = interval.start + offset,
                             .end = interval.end + offset};
}
