#pragma once
#include "./parser.h"
#include <stddef.h>

typedef struct GraphNode GraphNode;
struct GraphNode {
  GraphNode *left;
  GraphNode *right;
};

typedef struct {
  GraphNode *first;
  GraphNode *last;
} Graph;

size_t count_steps(Graph graph, char *path, size_t path_len);
Graph create_graph(ParsedInput parsed_input);
