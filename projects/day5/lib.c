#include "./lib.h"
#include "../file_utils/lib.h"
#include "../linked_list/lib.h"
#include "../parser_utils/lib.h"
#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

const size_t ranges_buffer_capacity = 64;
const size_t range_map_buffer_capacity = 64;
const size_t range_map_numbers_buffer_capacity = 64;
const size_t seeds_buffer_capacity = 128;
const size_t individual_number_buffer_capacity = 16;
const size_t path_buckets_buffer_capacity = 512;

void take_seeds(FileLines file_lines, size_t *current_line_idx,
                unsigned long *seeds_buffer, size_t *seeds_buffer_len) {
  char *line = file_lines.lines[*current_line_idx];
  char *current_position_within_line = line;
  take_string("seeds: ", &current_position_within_line);
  take_whitespace_separated_numbers(&current_position_within_line, seeds_buffer,
                                    seeds_buffer_len, seeds_buffer_capacity,
                                    individual_number_buffer_capacity);
  (*current_line_idx) += 2;
}

Range take_range(FileLines file_lines, size_t *current_line_idx) {
  char *line = file_lines.lines[(*current_line_idx)++];
  unsigned long dest_range_start = take_number(&line);
  maybe_take_whitespace(&line);
  unsigned long source_range_start = take_number(&line);
  maybe_take_whitespace(&line);
  unsigned long range_len = take_number(&line);
  return (Range){
      .dest_range_start = dest_range_start,
      .source_range_start = source_range_start,
      .range_len = range_len,
  };
}

void take_range_map(FileLines file_lines, size_t *current_line_idx,
                    RangeMap *range_map_buffer, size_t *range_map_buffer_len) {

  Range *ranges_buffer = malloc(ranges_buffer_capacity * sizeof(Range));
  if (ranges_buffer == 0) {
    printf("failed to allocate ranges buffer\n");
    exit(1);
  }
  size_t ranges_buffer_len = 0;

  (*current_line_idx)++;
  while (1) {
    if (*current_line_idx < file_lines.line_count) {
      char *line = file_lines.lines[*current_line_idx];
      if (isdigit(line[0])) {
        Range range = take_range(file_lines, current_line_idx);
        if (ranges_buffer_len == ranges_buffer_capacity) {
          printf("ranges buffer at capacity\n");
          exit(1);
        }
        ranges_buffer[ranges_buffer_len++] = range;
        continue;
      }
    }
    (*current_line_idx)++;
    break;
  }
  if (*range_map_buffer_len == range_map_buffer_capacity) {
    printf("range map buffer full\n");
    exit(EXIT_FAILURE);
  }
  range_map_buffer[(*range_map_buffer_len)++] = (RangeMap){
      .ranges = ranges_buffer,
      .ranges_len = ranges_buffer_len,
  };
}

void take_range_maps(FileLines file_lines, size_t *current_line_idx,
                     RangeMap *range_map_buffer, size_t *range_map_buffer_len) {
  while (*current_line_idx < file_lines.line_count) {
    take_range_map(file_lines, current_line_idx, range_map_buffer,
                   range_map_buffer_len);
  }
}

Almanac parse_input(char *input_path) {
  FileLines file_lines = read_file_lines(input_path);
  size_t current_line_idx = 0;

  unsigned long *seeds = malloc(seeds_buffer_capacity * sizeof(unsigned long));
  size_t seeds_len;
  take_seeds(file_lines, &current_line_idx, seeds, &seeds_len);

  RangeMap *range_maps = malloc(range_map_buffer_capacity * sizeof(RangeMap));
  size_t range_maps_len = 0;
  take_range_maps(file_lines, &current_line_idx, range_maps, &range_maps_len);

  return (Almanac){.seeds = seeds,
                   .seeds_len = seeds_len,
                   .range_maps = range_maps,
                   .range_maps_len = range_maps_len};
}

bool find_matching_range(RangeMap range_map, unsigned long value,
                         Range *result) {
  for (size_t i = 0; i < range_map.ranges_len; ++i) {
    Range range = range_map.ranges[i];
    if (value >= range.source_range_start &&
        value < range.source_range_start + range.range_len) {
      *result = range;
      return true;
    }
  }
  return false;
}

unsigned long map_seed_to_location(unsigned long seed, Almanac almanac) {
  unsigned long current_value = seed;
  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    RangeMap range_map = almanac.range_maps[i];
    Range matching_range;
    if (find_matching_range(range_map, current_value, &matching_range)) {
      unsigned long offset_within_range =
          current_value - matching_range.source_range_start;
      current_value = matching_range.dest_range_start + offset_within_range;
    }
  }
  return current_value;
}

unsigned long part1_lowest_location(Almanac almanac) {
  unsigned long min = ULONG_MAX;
  for (size_t i = 0; i < almanac.seeds_len; ++i) {
    unsigned long location = map_seed_to_location(almanac.seeds[i], almanac);
    if (location < min) {
      min = location;
    }
  }
  return min;
}

void print_path_bucket(PathBucket *path_bucket) {
  printf("path_bucket->seed_range_start: %lu\n", path_bucket->seed_range_start);
  printf("path_bucket->seed_range_end: %lu\n", path_bucket->seed_range_end);
  printf("path_bucket->current_range_start: %lu\n",
         path_bucket->current_range_start);
  printf("path_bucket->current_range_end: %lu\n",
         path_bucket->current_range_end);
  printf("\n");
}

void split_path_bucket(LinkedListNode *path_bucket_node, Range range) {
  PathBucket *path_bucket = (PathBucket *)(path_bucket_node->data);
  // there is always a shifted portion - we'll update the existing node to
  // become this
  unsigned long shifted_portion_start =
      MAX(path_bucket->current_range_start, range.source_range_start);
  unsigned long shifted_portion_end =
      MIN(path_bucket->current_range_end,
          range.source_range_start + range.range_len);

  unsigned long seed_range_start_truncation =
      shifted_portion_start - path_bucket->current_range_start;
  unsigned long seed_range_end_truncation =
      path_bucket->current_range_end - shifted_portion_end;

  PathBucket *shifted_path_bucket = malloc(sizeof(PathBucket));
  if (shifted_path_bucket == 0) {
    printf("failed to allocate shifted path bucket\n");
    exit(EXIT_FAILURE);
  }
  LinkedListNode *shifted_path_bucket_node = malloc(sizeof(LinkedListNode));
  if (shifted_path_bucket_node == 0) {
    printf("failed to allocate linked list ndoe\n");
    exit(EXIT_FAILURE);
  }

  signed long original_bucket_offset =
      (signed long)(path_bucket->current_range_start) -
      (signed long)(path_bucket->seed_range_start);

  signed long range_offset = (signed long)range.dest_range_start -
                             (signed long)range.source_range_start;

  signed long new_offset = original_bucket_offset + range_offset;

  unsigned long seed_range_start =
      path_bucket->seed_range_start + seed_range_start_truncation;

  unsigned long seed_range_end =
      path_bucket->seed_range_end - seed_range_end_truncation;

  *shifted_path_bucket = (PathBucket){
      .seed_range_start = seed_range_start,
      .seed_range_end = seed_range_end,
      .current_range_start =
          (unsigned long)((signed long)seed_range_start + new_offset),
      .current_range_end =
          (unsigned long)((signed long)seed_range_end + new_offset),
  };

  shifted_path_bucket_node->data = shifted_path_bucket;
  insert_after(path_bucket_node, shifted_path_bucket_node);

  if (range.source_range_start > path_bucket->current_range_start) {
    PathBucket *unshifted_start_bucket = malloc(sizeof(PathBucket));
    if (unshifted_start_bucket == 0) {
      printf("failed to allocate unshifted start bucket\n");
      exit(EXIT_FAILURE);
    }
    LinkedListNode *unshifted_start_bucket_node =
        malloc(sizeof(LinkedListNode));
    if (unshifted_start_bucket_node == 0) {
      printf("failed to allocate linked list ndoe\n");
      exit(EXIT_FAILURE);
    }

    unsigned long bucket_length =
        range.source_range_start - path_bucket->current_range_start;

    *unshifted_start_bucket = (PathBucket){
        .seed_range_start = path_bucket->seed_range_start,
        .seed_range_end = path_bucket->seed_range_start + bucket_length,
        .current_range_start = path_bucket->current_range_start,
        .current_range_end = path_bucket->current_range_start + bucket_length,
    };

    unshifted_start_bucket_node->data = unshifted_start_bucket;
    insert_after(path_bucket_node, unshifted_start_bucket_node);
  }
  if (range.source_range_start + range.range_len <
      path_bucket->current_range_end) {
    PathBucket *unshifted_end_bucket = malloc(sizeof(PathBucket));
    if (unshifted_end_bucket == 0) {
      printf("failed to allocate unshifted end bucket\n");
      exit(EXIT_FAILURE);
    }
    LinkedListNode *unshifted_end_bucket_node = malloc(sizeof(LinkedListNode));
    if (unshifted_end_bucket_node == 0) {
      printf("failed to allocate linked list ndoe\n");
      exit(EXIT_FAILURE);
    }

    unsigned long bucket_length = path_bucket->current_range_end -
                                  (range.source_range_start + range.range_len);

    *unshifted_end_bucket = (PathBucket){
        .seed_range_start = path_bucket->seed_range_end - bucket_length,
        .seed_range_end = path_bucket->seed_range_end,
        .current_range_start = path_bucket->current_range_end - bucket_length,
        .current_range_end = path_bucket->current_range_end,
    };

    unshifted_end_bucket_node->data = unshifted_end_bucket;
    insert_after(path_bucket_node, unshifted_end_bucket_node);
  }

  remove_node(path_bucket_node);
}

void update_or_split_path_bucket(LinkedListNode *path_bucket_node,
                                 Range range) {
  PathBucket *path_bucket = (PathBucket *)(path_bucket_node->data);
  bool range_covers_bucket =
      range.source_range_start <= path_bucket->current_range_start &&
      (range.source_range_start + range.range_len) >=
          path_bucket->current_range_end;

  if (range_covers_bucket) {
    unsigned long offset = range.dest_range_start - range.source_range_start;
    path_bucket->current_range_start += offset;
    path_bucket->current_range_end += offset;
  } else {
    bool range_start_is_within_bucket =
        range.source_range_start >= path_bucket->current_range_start &&
        range.source_range_start < path_bucket->current_range_end;
    bool range_end_is_within_bucket =
        (range.source_range_start + range.range_len) >
            path_bucket->current_range_start &&
        (range.source_range_start + range.range_len) <=
            path_bucket->current_range_end;

    bool range_overlaps_bucket =
        range_start_is_within_bucket || range_end_is_within_bucket;
    if (range_overlaps_bucket) {
      split_path_bucket(path_bucket_node, range);
    }
  }
}

void update_or_split_path_buckets(LinkedListNode *path_bucket_list_head,
                                  Range range) {
  LinkedListNode *current_path_bucket = path_bucket_list_head;
  while (current_path_bucket) {
    update_or_split_path_bucket(current_path_bucket, range);
    current_path_bucket = current_path_bucket->next;
  }
}

LinkedListNode *get_path_buckets(Almanac almanac) {
  PathBucket *initial_path_bucket = malloc(sizeof(PathBucket));
  if (initial_path_bucket == 0) {
    printf("failed to allocate initial path bucket\n");
    exit(EXIT_FAILURE);
  }

  *initial_path_bucket = (PathBucket){
      .seed_range_start = 0,
      .seed_range_end = ULONG_MAX,
      .current_range_start = 0,
      .current_range_end = ULONG_MAX,
  };

  LinkedListNode *path_bucket_list_head = malloc(sizeof(LinkedListNode));
  if (path_bucket_list_head == 0) {
    printf("failed to allocate initial path bucket list head\n");
    exit(EXIT_FAILURE);
  }

  *path_bucket_list_head = (LinkedListNode){.data = initial_path_bucket};

  for (size_t i = 0; i < almanac.range_maps_len; ++i) {
    for (size_t j = 0; j < almanac.range_maps[i].ranges_len; ++j) {
      update_or_split_path_buckets(path_bucket_list_head,
                                   almanac.range_maps[i].ranges[j]);
    }
  }
  return path_bucket_list_head;
}

unsigned long part2_lowest_location(Almanac almanac) {
  LinkedListNode *path_bucket_list_head = get_path_buckets(almanac);
  unsigned long min = ULONG_MAX;
  for (size_t i = 0; i < almanac.seeds_len; i += 2) {
    unsigned long seed_range_start = almanac.seeds[i];
    unsigned long seed_range_len = almanac.seeds[i + 1];
    unsigned long seed_range_end = seed_range_start + seed_range_len;

    LinkedListNode *current_node = path_bucket_list_head;
    while (current_node) {
      PathBucket *current_bucket = (PathBucket *)(current_node->data);
      print_path_bucket(current_bucket);
      signed long bucket_offset =
          (signed long)(current_bucket->current_range_start) -
          (signed long)(current_bucket->seed_range_start);

      unsigned long best_result_for_bucket = ULONG_MAX;
      if (seed_range_start <= current_bucket->seed_range_start &&
          seed_range_end > current_bucket->seed_range_start) {
        best_result_for_bucket = current_bucket->current_range_start;
      } else if (seed_range_start >= current_bucket->current_range_start &&
                 seed_range_start < seed_range_end) {
        best_result_for_bucket =
            (unsigned long)((signed long)seed_range_start + bucket_offset);
      }
      min = MIN(min, best_result_for_bucket);

      current_node = current_node->next;
    }
  }
  return min;
}
