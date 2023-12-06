#include <stddef.h>

typedef struct {
  unsigned long dest_range_start;
  unsigned long source_range_start;
  unsigned long range_len;
} Range;

typedef struct {
  Range *ranges;
  size_t ranges_len;
} RangeMap;

typedef struct {
  unsigned long *seeds;
  size_t seeds_len;
  RangeMap *range_maps;
  size_t range_maps_len;
} Almanac;

typedef struct {
  unsigned long seed_range_start;
  unsigned long seed_range_end;
  unsigned long current_range_start;
  unsigned long current_range_end;
} PathBucket;

/*

  start with path bucket of 0 -> ?, offset 0, current range 0 -> ?
  for each range map:
    for each range within range map:
      split existing path bucket

  then take seed ranges, and split into buckets with input seed range
  for each seed range bucket, take the top seed value from input seed range
  find min of these

  */

Almanac parse_input(char *input_path);
unsigned long part1_lowest_location(Almanac almanac);
unsigned long part2_lowest_location(Almanac almanac);
