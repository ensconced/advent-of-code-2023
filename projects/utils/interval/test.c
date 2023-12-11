#include "./lib.h"
#include <assert.h>
#include <stdio.h>

void test_contains(void) {
  interval_interval a = {.start = 100, .end = 200};
  interval_interval b = {.start = 150, .end = 250};
  interval_interval c = {.start = 50, .end = 250};
  assert(interval_contains(a, b) == false);
  assert(interval_contains(b, a) == false);
  assert(interval_contains(c, a) == true);
  assert(interval_contains(a, c) == false);
}

void test_equals(void) {
  interval_interval a = {.start = 0, .end = 0};
  interval_interval b = {.start = 0, .end = 0};
  assert(interval_equal(a, b) == true);

  interval_interval c = {.start = 100, .end = 200};
  interval_interval d = {.start = 100, .end = 200};
  assert(interval_equal(c, d) == true);

  interval_interval e = {.start = 100, .end = 200};
  interval_interval f = {.start = 100, .end = 201};
  assert(interval_equal(e, f) == false);
}

void test_is_empty(void) {
  interval_interval a = {.start = 0, .end = 0};
  assert(interval_is_empty(a) == true);

  interval_interval b = {.start = 0, .end = 100};
  assert(interval_is_empty(b) == false);

  interval_interval c = {.start = 100, .end = 100};
  assert(interval_is_empty(c) == true);

  interval_interval d = {.start = 100, .end = -100};
  assert(interval_is_empty(d) == true);
}

void test_intersection(void) {
  interval_interval a = {.start = 0, .end = 100};

  interval_interval b = {.start = 50, .end = 150};
  assert(interval_equal(interval_intersection(a, b),
                        (interval_interval){.start = 50, .end = 100}));

  interval_interval c = {.start = -50, .end = 50};
  assert(interval_equal(interval_intersection(a, c),
                        (interval_interval){.start = 0, .end = 50}));

  interval_interval d = {.start = -50, .end = 150};
  assert(interval_equal(interval_intersection(a, d),
                        (interval_interval){.start = 0, .end = 100}));

  interval_interval e = {.start = 0, .end = 0};
  assert(interval_is_empty(interval_intersection(a, e)));

  interval_interval f = {.start = 1000, .end = 1000};
  assert(interval_is_empty(interval_intersection(a, f)));

  interval_interval g = {.start = 20, .end = 80};
  assert(interval_equal(interval_intersection(a, g),
                        (interval_interval){.start = 20, .end = 80}));
}

void test_difference(void) {
  interval_interval a = {.start = 100, .end = 200};
  interval_interval b = {.start = 0, .end = 0};
  interval_difference_result a_without_b = interval_difference(a, b);
  assert(a_without_b.type == INTERVAL_SINGLE);
  assert(interval_equal(a, a_without_b.data.single));

  interval_interval c = {.start = 50, .end = 150};
  interval_difference_result a_without_c = interval_difference(a, c);
  assert(a_without_c.type == INTERVAL_SINGLE);
  assert(interval_equal(a_without_c.data.single,
                        (interval_interval){.start = 150, .end = 200}));

  interval_interval d = {.start = 150, .end = 250};
  interval_difference_result a_without_d = interval_difference(a, d);
  assert(a_without_d.type == INTERVAL_SINGLE);
  assert(interval_equal(a_without_d.data.single,
                        (interval_interval){.start = 100, .end = 150}));

  interval_interval e = {.start = 100, .end = 200};
  interval_difference_result a_without_e = interval_difference(a, e);
  assert(a_without_e.type == INTERVAL_SINGLE);
  assert(interval_is_empty(a_without_e.data.single));

  interval_interval f = {.start = 0, .end = 300};
  interval_difference_result a_without_f = interval_difference(a, f);
  assert(a_without_f.type == INTERVAL_SINGLE);
  assert(interval_is_empty(a_without_f.data.single));

  interval_interval g = {.start = 120, .end = 180};
  interval_difference_result a_without_g = interval_difference(a, g);
  assert(a_without_g.type == INTERVAL_PAIR);
  assert(interval_equal(a_without_g.data.pair[0],
                        (interval_interval){.start = 100, .end = 120}));
  assert(interval_equal(a_without_g.data.pair[1],
                        (interval_interval){.start = 180, .end = 200}));
}

int main(void) {
  printf("testing interval lib\n");
  test_contains();
  test_equals();
  test_is_empty();
  test_intersection();
  test_difference();
}
