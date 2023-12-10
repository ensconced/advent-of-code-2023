#include "./part2.h"
#include "./lib.h"
#include "./parser.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void advance_ghost_in_cycle_phase_to_next_end_node(Ghost *ghost, char *path,
                                                   size_t path_len) {
  printf("TODO - ghost: %p, path: %s, path_len: %zu\n", (void *)ghost, path,
         path_len);
}

void advance_ghost_in_tail_phase(Ghost *ghost, char *path, size_t path_len) {
  advance(&ghost->current_node, path, path_len);
  advance(&ghost->details.cycle_detection_pointer, path, path_len);
  advance(&ghost->details.cycle_detection_pointer, path, path_len);
  if (mega_graph_nodes_are_equal(&ghost->current_node,
                                 &ghost->details.cycle_detection_pointer,
                                 path_len)) {
    // we found a cycle
    printf("found a cycle!\n");
    // TODO - find the cycle, with all the end nodes within it.
    // transition the ghost into the cycle phase, and fulfil the intent of this
    // function call by advancing it to the next end node within the cycle
  }
}

void advance_ghost_in_tail_phase_to_next_end_node(Ghost *ghost, char *path,
                                                  size_t path_len) {
  while (!ghost->current_node.graph_node->is_end_node) {
    advance_ghost_in_tail_phase(ghost, path, path_len);
  }
}

void advance_ghost_to_next_end_node(Ghost *ghost, char *path, size_t path_len) {
  if (ghost->phase == TAIL) {
    advance_ghost_in_tail_phase_to_next_end_node(ghost, path, path_len);
  } else if (ghost->phase == CYCLE) {
    advance_ghost_in_cycle_phase_to_next_end_node(ghost, path, path_len);
  } else {
    printf("expected ghost to be either in tail phase or cycle phase\n");
    exit(EXIT_FAILURE);
  }
}

void advance_ghost_up_to_or_past_timestamp(Ghost *ghost, size_t timestamp,
                                           char *path, size_t path_len) {
  while (ghost->current_node.timestamp < timestamp) {
    advance_ghost_to_next_end_node(ghost, path, path_len);
  }
}

Ghost *create_ghosts(GraphNode *graph_nodes, ParsedInput parsed_input) {
  static const size_t ghosts_capacity = 32;
  Ghost *ghosts = malloc(ghosts_capacity * sizeof(Ghost));
  size_t ghosts_len = 0;
  if (ghosts == 0) {
    printf("failed to allocate ghosts\n");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
    if (graph_nodes[i].is_start_node) {
      if (ghosts_len == ghosts_capacity - 1) {
        printf("ran out of capacity in ghosts buffer\n");
        exit(EXIT_FAILURE);
      }

      ghosts[ghosts_len++] = (Ghost){
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

size_t part2(char *input_path) {
  ParsedInput parsed_input = parse_input(input_path);
  size_t path_len = strlen(parsed_input.path);
  GraphNode *graph_nodes = create_graph(parsed_input, true);
  Ghost *ghosts = create_ghosts(graph_nodes, parsed_input);

  size_t max_timestamp = 0;
  while (1) {
    bool all_timestamps_match = true;
    for (size_t i = 0; i < parsed_input.nodes_len; ++i) {
      Ghost *ghost = &ghosts[i];
      advance_ghost_up_to_or_past_timestamp(ghost, max_timestamp,
                                            parsed_input.path, path_len);
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
