#include <stdbool.h>
#include <stddef.h>

void take_word(char *word, char **str_pointer);
bool maybe_take_word(char *word, char **str_pointer);
void take_natural_number(char **str_pointer, char *result_buffer,
                         size_t result_buffer_capacity);
void maybe_take_natural_number(char **str_pointer, char *result_buffer,
                               size_t result_buffer_capacity);
