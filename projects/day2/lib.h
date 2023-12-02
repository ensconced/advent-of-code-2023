#include <stdbool.h>

enum Colour { BLUE, RED, GREEN };

void take_word(char *word, char **str_pointer);
bool maybe_take_word(char *word, char **str_pointer);
enum Colour take_colour(char **str_pointer);
int take_natural_number(char **str_pointer);
int take_header_returning_game_id(char **str_pointer);
int maybe_take_natural_number(char **str_pointer);
