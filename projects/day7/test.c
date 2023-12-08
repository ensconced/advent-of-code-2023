#include "./lib.h"
#include "./parser.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(part1("./inputs/example_input.txt") == 6440);
  assert(part1("./inputs/input.txt") == 250957639);
}
