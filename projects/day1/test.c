#include "./lib.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

int main(void) {
  int result = sum_calibration_values("./inputs/part1_example.txt", false);
  assert(result == 142);
  printf("part 1 result: %d\n",
         sum_calibration_values("./inputs/input.txt", false));
  int part2_example_result =
      sum_calibration_values("./inputs/part2_example.txt", true);
  printf("part 2 example result: %d\n", part2_example_result);
  assert(part2_example_result == 281);
  printf("part 2 result: %d\n",
         sum_calibration_values("./inputs/input.txt", true));
}
