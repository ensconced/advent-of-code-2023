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

typedef enum {
  TAIL,
  CYCLE,
} GhostPhase;

typedef struct {
  GraphNode *graph_node;
  size_t timestamp;
} MegaGraphPointer;

typedef struct {
  size_t *steps;
  size_t steps_len;
  size_t next_step_idx;
} CyclePhaseDetails;

typedef union {
  MegaGraphPointer cycle_detection_pointer;
  CyclePhaseDetails cycle_phase_details;
} PhaseDetails;

typedef struct {
  GhostPhase phase;
  GraphNode *start_node;
  MegaGraphPointer current_node;
  PhaseDetails details;
} Ghost;

size_t find_node_idx_by_id(ParsedInput parsed_input, char *id);
GraphNode *create_graph(ParsedInput parsed_input, bool part2);
size_t get_result(char *input_path, bool part2);
