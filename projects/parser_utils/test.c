
#include "./lib.h"
#include <assert.h>
#include <stdbool.h>
#include <string.h>

void test_take_string(void) {
  char *str = "hello there";
  take_string("hello ", &str);
  assert(strcmp(str, "there") == 0);
}

void test_maybe_take_string(void) {
  char *str1 = "hello there";
  bool result1 = maybe_take_string("hello", &str1);
  assert(result1 == true);
  assert(strcmp(str1, " there") == 0);

  char *str2 = "hi there";
  bool result2 = maybe_take_string("hello", &str2);
  assert(result2 == false);
  assert(strcmp(str2, "hi there") == 0);
}

void test_take_numeric_string(void) {
  char *str1 = "1234 hello there";
  static const size_t number_buffer_len = 16;
  char number_buffer[number_buffer_len];
  take_numeric_string(&str1, number_buffer, number_buffer_len);
  assert(strcmp(number_buffer, "1234") == 0);
  assert(strcmp(str1, " hello there") == 0);

  char *str2 = "00001 hello there";
  take_numeric_string(&str2, number_buffer, number_buffer_len);
  assert(strcmp(number_buffer, "00001") == 0);
  assert(strcmp(str2, " hello there") == 0);
}

void test_take_number(void) {
  char *str1 = "1234 hello there";
  int result = take_number(&str1);
  assert(result == 1234);
  assert(strcmp(str1, " hello there") == 0);
}

void test_maybe_take_numeric_string(void) {
  char *str1 = "foo bar 1234";
  static const size_t number_buffer_capacity = 16;
  char number_buffer[number_buffer_capacity];
  maybe_take_numeric_string(&str1, number_buffer, number_buffer_capacity);
  assert(strcmp(number_buffer, "") == 0);
  assert(strcmp(str1, "foo bar 1234") == 0);

  char *str2 = "1234 foo bar";
  maybe_take_numeric_string(&str2, number_buffer, number_buffer_capacity);
  assert(strcmp(number_buffer, "1234") == 0);
  assert(strcmp(str2, " foo bar") == 0);

  char *str3 = "00001 foo bar";
  maybe_take_numeric_string(&str3, number_buffer, number_buffer_capacity);
  assert(strcmp(number_buffer, "00001") == 0);
  assert(strcmp(str3, " foo bar") == 0);
}

void test_maybe_take_whitespace(void) {
  char *str1 = " \t \t foo bar";
  bool took1 = maybe_take_whitespace(&str1);
  assert(took1);
  assert(strcmp(str1, "foo bar") == 0);

  char *str2 = "foo bar";
  bool took2 = maybe_take_whitespace(&str2);
  assert(!took2);
  assert(strcmp(str2, "foo bar") == 0);
}

void test_take_whitespace_separated_numeric_strings(void) {
  char *str1 = "123 \t \t 456 \t \t 789 foo bar";
  static const size_t result_buffer_capacity = 4;
  static const size_t number_buffer_capacity = 16;
  size_t result_buffer_len;
  char *result_buffer[result_buffer_capacity];
  take_whitespace_separated_numeric_strings(
      &str1, result_buffer, &result_buffer_len, result_buffer_capacity,
      number_buffer_capacity);
  assert(strcmp(str1, "foo bar") == 0);
  assert(result_buffer_len == 3);

  char *expected_result1 = "123";
  char *expected_result2 = "456";
  char *expected_result3 = "789";

  char *expected_result_buffer[] = {expected_result1, expected_result2,
                                    expected_result3};

  for (size_t i = 0; i < result_buffer_len; ++i) {
    assert(strcmp(result_buffer[i], expected_result_buffer[i]) == 0);
  }
}

void test_take_whitespace_separated_numbers(void) {
  char *str = "123  456  789";
  static const size_t result_buffer_capacity = 4;
  static const size_t individual_number_buffer_capacity = 8;
  int result_buffer[result_buffer_capacity];
  size_t result_buffer_len;
  take_whitespace_separated_numbers(&str, result_buffer, &result_buffer_len,
                                    result_buffer_capacity,
                                    individual_number_buffer_capacity);

  static const size_t expected_len = 3;
  int expected_result[expected_len] = {123, 456, 789};
  assert(result_buffer_len == expected_len);
  for (size_t i = 0; i < expected_len; ++i) {
    assert(result_buffer[i] == expected_result[i]);
  }
}

int main(void) {
  test_take_string();
  test_maybe_take_string();
  test_take_numeric_string();
  test_take_number();
  test_maybe_take_numeric_string();
  test_maybe_take_whitespace();
  test_take_whitespace_separated_numeric_strings();
  test_take_whitespace_separated_numbers();
}
