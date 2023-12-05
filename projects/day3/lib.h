#include <stdbool.h>
#include <stddef.h>

typedef struct {
  size_t x;
  size_t y;
} Coordinates;

typedef struct {
  Coordinates coordinates;
  char character;
} Symbol;

typedef struct {
  Coordinates start_coords;
  size_t str_len;
  int num;
} PositionedNumber;

typedef struct {
  Symbol *symbols;
  size_t symbols_len;
  PositionedNumber *numbers;
  size_t numbers_len;
} Grid;

Grid parse_input(char *input_path);
int sum_part_numbers(Grid grid);
int sum_gear_ratios(Grid grid);
void free_grid(Grid grid);
