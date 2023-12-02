#include <stdbool.h>
#include <stddef.h>

enum Colour { BLUE, RED, GREEN };

typedef struct Round {
  int red;
  int blue;
  int green;
} Round;

typedef struct Game {
  Round **rounds;
  size_t round_count;
  int id;
} Game;

void take_word(char *word, char **str_pointer);
bool maybe_take_word(char *word, char **str_pointer);
enum Colour take_colour(char **str_pointer);
int take_natural_number(char **str_pointer);
int take_header_returning_game_id(char **str_pointer);
int maybe_take_natural_number(char **str_pointer);
Round *maybe_take_round(char **str_pointer);
