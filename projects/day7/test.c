#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  printf("testing day7\n");
  assert(get_result("./inputs/example_input.txt", false) == 6440);
  assert(get_result("./inputs/input.txt", false) == 250957639);
  assert(get_result("./inputs/example_input.txt", true) == 5905);
  assert(get_result("./inputs/input.txt", true) == 251515496);
}
