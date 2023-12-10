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

ParsedInput parse_input(char *input_path);
