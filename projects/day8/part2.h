#pragma once
#include "./lib.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
  TAIL,
  CYCLE,
} GhostPhase;

typedef struct {
  GraphNode *graph_node;
  size_t timestamp;
} MegaGraphPointer;

// typedef struct {
// } CyclePhaseDetails;

typedef union {
  MegaGraphPointer cycle_detection_pointer;
  // CyclePhaseDetails cycle;
} PhaseDetails;

typedef struct {
  GhostPhase phase;
  MegaGraphPointer current_node;
  PhaseDetails details;
} Ghost;
