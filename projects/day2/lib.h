#include <stdbool.h>
#include <stddef.h>

enum Colour { BLUE, RED, GREEN };

typedef struct Round {
  int red;
  int green;
  int blue;
} Round;

typedef struct Game {
  Round **rounds;
  size_t round_count;
  int id;
} Game;

enum Colour take_colour(char **str_pointer);
int take_header_returning_game_id(char **str_pointer);
Round *maybe_take_round(char **str_pointer);
Round **take_rounds(size_t *round_count_result, char **str_pointer);
Game parse_line(char *line);
int sum_possible_game_ids(char *input_path);
int sum_game_powers(char *input_path);
void free_game(Game game);
