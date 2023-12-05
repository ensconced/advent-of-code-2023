#include "./lib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  assert(sum_calibration_values("./inputs/part1_example.txt", false) == 142);
  assert(sum_calibration_values("./inputs/input.txt", false) == 55208);
  assert(sum_calibration_values("./inputs/part2_example.txt", true) == 281);
  assert(sum_calibration_values("./inputs/input.txt", true) == 54578);
}
