#include "./parser.h"
#include "./part1.h"
#include <assert.h>
#include <stdio.h>

int main(void) {
  assert(part1("./inputs/example_input.txt") == 2);
  assert(part1("./inputs/input.txt") == 18113);
}
