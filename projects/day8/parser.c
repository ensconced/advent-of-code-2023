#include "../utils/file_utils/lib.h"
#include "../utils/parser_utils/lib.h"
#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>

const size_t id_buffer_capacity = 8;
const size_t path_buffer_capacity = 512;
const size_t nodes_capacity = 1028;

char *take_path(char *line) {
  char *line_pointer = line;
  char *path = malloc(path_buffer_capacity);
  take_alphanumeric_string(&line_pointer, path, path_buffer_capacity);
  return path;
}

ParsedNode parse_line(char *line) {
  char *line_pointer = line;

  char *id = malloc(id_buffer_capacity);
  take_alphanumeric_string(&line_pointer, id, id_buffer_capacity);

  take_string(" = (", &line_pointer);

  char *left = malloc(id_buffer_capacity);
  take_alphanumeric_string(&line_pointer, left, id_buffer_capacity);

  take_string(", ", &line_pointer);

  char *right = malloc(id_buffer_capacity);
  take_alphanumeric_string(&line_pointer, right, id_buffer_capacity);

  return (ParsedNode){
      .id = id,
      .left = left,
      .right = right,
  };
}

ParsedInput parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  char *path = take_path(file_lines.lines[0]);

  ParsedNode *nodes = malloc(nodes_capacity * sizeof(ParsedNode));
  size_t nodes_len = 0;
  for (size_t i = 2; i < file_lines.line_count; ++i) {
    ParsedNode node = parse_line(file_lines.lines[i]);
    if (nodes_len == nodes_capacity) {
      printf("ran out of nodes capacity\n");
      exit(EXIT_FAILURE);
    }
    nodes[nodes_len++] = node;
  }
  return (ParsedInput){
      .nodes_len = nodes_len,
      .nodes = nodes,
      .path = path,
  };
}
