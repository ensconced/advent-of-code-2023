#include "./lib.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  int part1_result = part1("./inputs/input.txt");
  printf("part 1 result: %d\n", part1_result);

  int part2_result = part2("./inputs/input.txt");
  printf("part 2 result: %d\n", part2_result);
}
