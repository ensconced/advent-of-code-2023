
#include "./lib.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void test_take_word(void) {
  char *str = "hello there";
  take_word("hello ", &str);
  assert(strcmp(str, "there") == 0);
}

void test_maybe_take_word(void) {
  char *str1 = "hello there";
  bool result1 = maybe_take_word("hello", &str1);
  assert(result1 == true);
  assert(strcmp(str1, " there") == 0);

  char *str2 = "hi there";
  bool result2 = maybe_take_word("hello", &str2);
  assert(result2 == false);
  assert(strcmp(str2, "hi there") == 0);
}

void test_take_natural_number(void) {
  char *str = "1234 hello there";
  static const size_t number_buffer_len = 16;
  char number_buffer[number_buffer_len];
  take_natural_number(&str, number_buffer, number_buffer_len);
  assert(strcmp(number_buffer, "1234") == 0);
  assert(strcmp(str, " hello there") == 0);
}

void test_maybe_take_natural_number(void) {
  char *str1 = "foo bar 1234";
  static const size_t number_buffer_capacity = 16;
  char number_buffer[number_buffer_capacity];
  maybe_take_natural_number(&str1, number_buffer, number_buffer_capacity);
  assert(strcmp(number_buffer, "") == 0);
  assert(strcmp(str1, "foo bar 1234") == 0);

  char *str2 = "1234 foo bar";
  maybe_take_natural_number(&str2, number_buffer, number_buffer_capacity);
  assert(strcmp(number_buffer, "1234") == 0);
  assert(strcmp(str2, " foo bar") == 0);
}

int main(void) {
  test_take_word();
  test_maybe_take_word();
  test_take_natural_number();
  test_maybe_take_natural_number();
}
