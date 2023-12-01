#include "./lib.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  int result = sum_calibration_values("example_input.txt");
  assert(result == 142);
  printf("%d", sum_calibration_values("input.txt"));
}
