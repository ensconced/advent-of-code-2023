#include <stddef.h>

typedef struct {
  int dest_range_start;
  int source_range_start;
  int range_len;
} Range;

typedef struct {
  Range *ranges;
  size_t ranges_len;
} RangeMap;

typedef struct {
  int *seeds;
  size_t seeds_len;
  RangeMap *range_maps;
  size_t range_maps_len;
} Almanac;

Almanac parse_input(char *input_path);
