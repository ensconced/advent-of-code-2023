#include "./lib.h"
#include "./parser.h"
#include <stddef.h>
#include <string.h>

/*

typedef enum {
  TAIL,
  CYCLE,
} GhostPhase;

typedef struct {
  size_t tortoise_timestamp;
  GraphNode * tortoise_node;
  size_t hare_timestamp;
  GraphNode * hare_node;
} TailPhaseDetails;

typedef struct {
} CyclePhaseDetails;

typedef union {
  TailPhaseDetails tail;
  CyclePhaseDetails cycle;
} PhaseDetails;

typedef struct {
  GhostPhase phase;
  PhaseDetails details;
} Ghost;


void advance_ghost_in_tail_phase() {
  advance_tortoise(ghost);
  advance_hare(ghost);
}

void advance_ghost_in_cycle_phase() {
  advance_tortoise(ghost);
  advance_hare(ghost);
}

void advance_ghost(Ghost* ghost) {
  if (ghost->phase == TAIL) {
    advance_ghost_in_tail_phase(ghost);
    if (tortoise_and_hare_at_same_node(ghost)) {
      move_to_cycle_phase(ghost);
    }
  } else if (ghost->phase == CYCLE) {
    advance_ghost_in_cycle_phase(ghost);
  } else {
    error
  }
}


*/

size_t part2(char *input_path) {
  ParsedInput parsed_input = parse_input(input_path);
  GraphNode *graph_nodes = create_graph(parsed_input);

  /*

  find all start nodes
  find all end nodes

  for each start node
    create ghost. a ghost is either a tail ghost or a cycle ghost. they all
    start as tail ghosts.

  size_t max_timestamp = 0;

  while (1) {
    bool all_timestamps_match = true;
    for each ghost {
      while (1) {
        advance_ghost_to_next_end_node(ghost);
        if (ghost->timestamp >= max_timestamp) {
          max_timestamp = ghost->timestamp;
          break;
        }
      }
    }
    if (all_timestamps_match) {
      return max_timestamp;
    }
  }
  */
}
