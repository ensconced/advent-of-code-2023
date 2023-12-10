#include "./lib.h"
#include "./parser.h"
#include <stddef.h>
#include <string.h>

size_t part1(char *input_path) {
  ParsedInput parsed_input = parse_input(input_path);
  GraphNode *graph_nodes = create_graph(parsed_input, false);
  GraphNode *start_node =
      &graph_nodes[find_node_idx_by_id(parsed_input, "AAA")];
  GraphNode *end_node = &graph_nodes[find_node_idx_by_id(parsed_input, "ZZZ")];
  return count_steps(start_node, end_node, parsed_input.path);
}
