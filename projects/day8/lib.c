#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t find_node_idx_by_id(ParsedInput parsed_input, char *id) {
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    ParsedNode node = parsed_input.nodes[i];
    if (strcmp(node.id, id) == 0)
      return i;
  }
  printf("failed to find node\n");
  exit(EXIT_FAILURE);
}

GraphNode *create_graph(ParsedInput parsed_input, bool part2) {
  GraphNode *graph_nodes = malloc(parsed_input.nodes_len * sizeof(GraphNode));
  if (graph_nodes == 0) {
    printf("failed to allocate graph nodes\n");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    ParsedNode parsed_node = parsed_input.nodes[i];

    graph_nodes[i] = (GraphNode){
        .left = 0,
        .right = 0,
        .is_start_node = part2 ? parsed_node.id[2] == 'A'
                               : strcmp(parsed_node.id, "AAA") == 0,
        .is_end_node = part2 ? parsed_node.id[2] == 'Z'
                             : strcmp(parsed_node.id, "ZZZ") == 0,
    };
  }
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    ParsedNode parsed_node = parsed_input.nodes[i];
    size_t left_idx = find_node_idx_by_id(parsed_input, parsed_node.left);
    graph_nodes[i].left = &graph_nodes[left_idx];
    size_t right_idx = find_node_idx_by_id(parsed_input, parsed_node.right);
    graph_nodes[i].right = &graph_nodes[right_idx];
  }

  return graph_nodes;
}

bool mega_graph_nodes_are_equal(MegaGraphPointer *a, MegaGraphPointer *b,
                                size_t path_len) {
  return a->graph_node == b->graph_node &&
         a->timestamp % path_len == b->timestamp % path_len;
}

void advance(MegaGraphPointer *pointer, char *path, size_t path_len) {
  char direction = path[pointer->timestamp % path_len];
  pointer->graph_node =
      direction == 'L' ? pointer->graph_node->left : pointer->graph_node->right;
  pointer->timestamp++;
}

void advance_ghost_in_cycle_phase_to_next_end_node(Ghost *ghost) {
  ghost->current_node.timestamp +=
      ghost->details.cycle_phase_details
          .steps[ghost->details.cycle_phase_details.next_step_idx];

  ghost->details.cycle_phase_details.next_step_idx =
      (ghost->details.cycle_phase_details.next_step_idx + 1) %
      ghost->details.cycle_phase_details.steps_len;
}

void transition_ghost_to_cycle_phase(Ghost *ghost, char *path,
                                     size_t path_len) {
  while (!ghost->current_node.graph_node->is_end_node) {
    advance(&ghost->current_node, path, path_len);
  }

  MegaGraphPointer cycle_finder_pointer = ghost->current_node;

  static const size_t steps_capacity = 128;
  size_t *steps = malloc(steps_capacity * sizeof(size_t));
  if (steps == 0) {
    printf("failed to allocate steps");
    exit(EXIT_FAILURE);
  }
  size_t steps_len = 0;

  do {
    size_t step_size = 0;
    do {
      advance(&cycle_finder_pointer, path, path_len);
      step_size++;
    } while (!cycle_finder_pointer.graph_node->is_end_node);

    if (steps_len == steps_capacity - 1) {
      printf("ran out of capacity in steps buffer\n");
      exit(EXIT_FAILURE);
    }
    steps[steps_len++] = step_size;
  } while (!mega_graph_nodes_are_equal(&cycle_finder_pointer,
                                       &ghost->current_node, path_len));
  ghost->phase = CYCLE;
  ghost->details = (PhaseDetails){
      .cycle_phase_details =
          (CyclePhaseDetails){
              .steps = steps,
              .steps_len = steps_len,
              .next_step_idx = 0,
          },
  };
}

void advance_ghost_in_tail_phase(Ghost *ghost, char *path, size_t path_len) {
  advance(&ghost->current_node, path, path_len);
  advance(&ghost->details.cycle_detection_pointer, path, path_len);
  advance(&ghost->details.cycle_detection_pointer, path, path_len);
  if (mega_graph_nodes_are_equal(&ghost->current_node,
                                 &ghost->details.cycle_detection_pointer,
                                 path_len)) {
    transition_ghost_to_cycle_phase(ghost, path, path_len);
  }
}

void advance_ghost_in_tail_phase_to_next_end_node(Ghost *ghost, char *path,
                                                  size_t path_len) {
  do {
    advance_ghost_in_tail_phase(ghost, path, path_len);
  } while (!ghost->current_node.graph_node->is_end_node);
}

void advance_ghost_to_next_end_node(Ghost *ghost, char *path, size_t path_len) {
  if (ghost->phase == TAIL) {
    advance_ghost_in_tail_phase_to_next_end_node(ghost, path, path_len);
  } else if (ghost->phase == CYCLE) {
    advance_ghost_in_cycle_phase_to_next_end_node(ghost);
  } else {
    printf("expected ghost to be either in tail phase or cycle phase\n");
    exit(EXIT_FAILURE);
  }
}

void advance_through_end_nodes_up_to_or_past_timestamp(Ghost *ghost,
                                                       size_t target_timestamp,
                                                       char *path,
                                                       size_t path_len) {
  do {
    advance_ghost_to_next_end_node(ghost, path, path_len);
  } while (ghost->current_node.timestamp < target_timestamp);
}

Ghost *create_ghosts(GraphNode *graph_nodes, ParsedInput parsed_input,
                     size_t *ghosts_len) {
  static const size_t ghosts_capacity = 32;
  Ghost *ghosts = malloc(ghosts_capacity * sizeof(Ghost));
  if (ghosts == 0) {
    printf("failed to allocate ghosts\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    if (graph_nodes[i].is_start_node) {
      if (*ghosts_len == ghosts_capacity - 1) {
        printf("ran out of capacity in ghosts buffer\n");
        exit(EXIT_FAILURE);
      }

      ghosts[(*ghosts_len)++] = (Ghost){
          .start_node = &graph_nodes[i],
          .phase = TAIL,
          .current_node =
              (MegaGraphPointer){
                  .graph_node = &graph_nodes[i],
                  .timestamp = 0,
              },
          .details =
              (PhaseDetails){
                  .cycle_detection_pointer =
                      (MegaGraphPointer){
                          .graph_node = &graph_nodes[i],
                          .timestamp = 0,
                      },
              },
      };
    }
  }
  return ghosts;
}

size_t get_result(char *input_path, bool part2) {
  ParsedInput parsed_input = parse_input(input_path);
  size_t path_len = strlen(parsed_input.path);
  GraphNode *graph_nodes = create_graph(parsed_input, part2);

  size_t ghosts_len = 0;
  Ghost *ghosts = create_ghosts(graph_nodes, parsed_input, &ghosts_len);

  size_t max_timestamp = 0;
  while (1) {
    bool all_timestamps_match = true;
    for (size_t i = 0; i < ghosts_len; ++i) {
      Ghost *ghost = &ghosts[i];

      if (ghost->current_node.timestamp == 0 ||
          ghost->current_node.timestamp < max_timestamp) {
        advance_through_end_nodes_up_to_or_past_timestamp(
            ghost, max_timestamp, parsed_input.path, path_len);
      }

      max_timestamp = ghost->current_node.timestamp;

      if (i > 0 && ghost->current_node.timestamp !=
                       ghosts[i - 1].current_node.timestamp) {
        all_timestamps_match = false;
      }
    }

    if (all_timestamps_match)
      return max_timestamp;
  }
}
