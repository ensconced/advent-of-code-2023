#include "./lib.h"
#include <assert.h>

void test_contains(void) {
  Interval a = {.start = 100, .end = 200};
  Interval b = {.start = 150, .end = 250};
  Interval c = {.start = 50, .end = 250};
  assert(contains(a, b) == false);
  assert(contains(b, a) == false);
  assert(contains(c, a) == true);
  assert(contains(a, c) == false);
}

void test_equals(void) {
  Interval a = {.start = 0, .end = 0};
  Interval b = {.start = 0, .end = 0};
  assert(equals(a, b) == true);

  Interval c = {.start = 100, .end = 200};
  Interval d = {.start = 100, .end = 200};
  assert(equals(c, d) == true);

  Interval e = {.start = 100, .end = 200};
  Interval f = {.start = 100, .end = 201};
  assert(equals(e, f) == false);
}

void test_is_empty(void) {
  Interval a = {.start = 0, .end = 0};
  assert(is_empty(a) == true);

  Interval b = {.start = 0, .end = 100};
  assert(is_empty(b) == false);

  Interval c = {.start = 100, .end = 100};
  assert(is_empty(c) == true);

  Interval d = {.start = 100, .end = -100};
  assert(is_empty(d) == true);
}

void test_intersection(void) {
  Interval a = {.start = 0, .end = 100};

  Interval b = {.start = 50, .end = 150};
  assert(equals(intersection(a, b), (Interval){.start = 50, .end = 100}));

  Interval c = {.start = -50, .end = 50};
  assert(equals(intersection(a, c), (Interval){.start = 0, .end = 50}));

  Interval d = {.start = -50, .end = 150};
  assert(equals(intersection(a, d), (Interval){.start = 0, .end = 100}));

  Interval e = {.start = 0, .end = 0};
  assert(is_empty(intersection(a, e)));

  Interval f = {.start = 1000, .end = 1000};
  assert(is_empty(intersection(a, f)));

  Interval g = {.start = 20, .end = 80};
  assert(equals(intersection(a, g), (Interval){.start = 20, .end = 80}));
}

int main(void) {
  test_contains();
  test_equals();
  test_is_empty();
  test_intersection();
}
