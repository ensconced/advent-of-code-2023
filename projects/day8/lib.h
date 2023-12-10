#pragma once
#include "./parser.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct GraphNode GraphNode;
struct GraphNode {
  GraphNode *left;
  GraphNode *right;
  bool is_start_node;
  bool is_end_node;
};

size_t find_node_idx_by_id(ParsedInput parsed_input, char *id);
size_t count_steps(GraphNode *start_node, GraphNode *end_node, char *path);
GraphNode *create_graph(ParsedInput parsed_input, bool part2);
