#pragma once
#include <stddef.h>

typedef struct {
  char *id;
  char *left;
  char *right;
} ParsedNode;

typedef struct {
  ParsedNode *nodes;
  size_t nodes_len;
  char *path;
} ParsedInput;

typedef struct GraphNode GraphNode;
struct GraphNode {
  GraphNode *left;
  GraphNode *right;
};

typedef struct {
  GraphNode *first;
  GraphNode *last;
} Graph;

size_t get_result(char *input_path);
