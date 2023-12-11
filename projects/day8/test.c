#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day8\n");
  assert(get_result("./inputs/example_input.txt", false) == 2);
  assert(get_result("./inputs/input.txt", false) == 18113);
  assert(get_result("./inputs/part2_example_input.txt", true) == 6);
  assert(get_result("./inputs/input.txt", true) == 12315788159977);
}
