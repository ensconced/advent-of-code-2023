#include "./lib.h"
#include "./parser.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t count_steps(Graph graph, char *path, size_t path_len) {
  size_t count = 0;
  size_t path_idx = 0;
  GraphNode *current_node = graph.first;
  while (current_node != graph.last) {
    current_node =
        path[path_idx] == 'L' ? current_node->left : current_node->right;
    path_idx = (path_idx + 1) % path_len;
    count++;
  }
  return count;
}

size_t find_node_idx_by_id(ParsedInput parsed_input, char *id) {
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    ParsedNode node = parsed_input.nodes[i];
    if (strcmp(node.id, id) == 0)
      return i;
  }
  printf("failed to find node\n");
  exit(EXIT_FAILURE);
}

Graph create_graph(ParsedInput parsed_input) {
  GraphNode *graph_nodes = malloc(parsed_input.nodes_len * sizeof(GraphNode));
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    graph_nodes[i] = (GraphNode){.left = 0, .right = 0};
  }
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    ParsedNode parsed_node = parsed_input.nodes[i];
    size_t left_idx = find_node_idx_by_id(parsed_input, parsed_node.left);
    graph_nodes[i].left = &graph_nodes[left_idx];
    size_t right_idx = find_node_idx_by_id(parsed_input, parsed_node.right);
    graph_nodes[i].right = &graph_nodes[right_idx];
  }

  return (Graph){
      .first = &graph_nodes[find_node_idx_by_id(parsed_input, "AAA")],
      .last = &graph_nodes[find_node_idx_by_id(parsed_input, "ZZZ")],
  };
}

size_t get_result(char *input_path) {
  ParsedInput parsed_input = parse_input(input_path);
  Graph graph = create_graph(parsed_input);
  size_t path_len = strlen(parsed_input.path);
  return count_steps(graph, parsed_input.path, path_len);
}
