#include "./lib.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day4\n");
  assert(part1("./inputs/input.txt") == 25010);
  assert(part2("./inputs/input.txt") == 9924412);
}
