#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(get_result("./inputs/example_input.txt", false) == 2);
  assert(get_result("./inputs/input.txt", false) == 18113);
  assert(get_result("./inputs/part2_example_input.txt", true) == 6);
}
