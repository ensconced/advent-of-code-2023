#pragma once
#include "./parser.h"
#include <stddef.h>

typedef struct GraphNode GraphNode;
struct GraphNode {
  GraphNode *left;
  GraphNode *right;
};

size_t find_node_idx_by_id(ParsedInput parsed_input, char *id);
size_t count_steps(GraphNode *start_node, GraphNode *end_node, char *path);
GraphNode *create_graph(ParsedInput parsed_input);
