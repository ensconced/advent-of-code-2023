#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(get_result("./inputs/example_input.txt") == 2);
  assert(get_result("./inputs/input.txt") == 18113);
}
