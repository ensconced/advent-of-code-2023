#include "./lib.h"
#include "./parser.h"
#include <stddef.h>
#include <string.h>

size_t part1(char *input_path) {
  ParsedInput parsed_input = parse_input(input_path);
  Graph graph = create_graph(parsed_input);
  size_t path_len = strlen(parsed_input.path);
  return count_steps(graph, parsed_input.path, path_len);
}
