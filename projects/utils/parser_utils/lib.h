#pragma once
#include <stdbool.h>
#include <stddef.h>

void take_string(char *word, char **str_pointer);
bool maybe_take_string(char *word, char **str_pointer);
bool maybe_take_whitespace(char **str_pointer);
void take_numeric_string(char **str_pointer, char *result_buffer,
                         size_t result_buffer_capacity);
void take_alphanumeric_string(char **str_pointer, char *result_buffer,
                              size_t result_buffer_capacity);
unsigned long take_number(char **str_pointer);
void maybe_take_numeric_string(char *const *str_pointer, char *result_buffer,
                               size_t result_buffer_capacity);
void take_whitespace_separated_numeric_strings(char **str_pointer,
                                               char **result_buffer,
                                               size_t *result_buffer_len,
                                               size_t result_buffer_capacity,
                                               size_t number_buffer_capacity);
void take_whitespace_separated_numbers(char **str_pointer,
                                       unsigned long *result_buffer,
                                       size_t *result_buffer_len,
                                       size_t result_buffer_capacity,
                                       size_t number_buffer_capacity);
