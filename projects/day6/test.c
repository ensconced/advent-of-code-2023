#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day6\n");
  assert(part1("./inputs/example_input.txt") == 288);
  assert(part1("./inputs/input.txt") == 781200);
  assert(part2("./inputs/example_input.txt") == 71503);
  assert(part2("./inputs/input.txt") == 49240091);
}
