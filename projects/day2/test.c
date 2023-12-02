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
  int result = take_natural_number(&str);
  assert(result == 1234);
  assert(strcmp(str, " hello there") == 0);
}

void test_take_header_returning_game_id(void) {
  char *str = "Game 1234: 3 blue";
  int game_id = take_header_returning_game_id(&str);
  assert(game_id == 1234);
  assert(strcmp(str, "3 blue") == 0);
}

void test_maybe_take_natural_number(void) {
  char *str1 = "foo bar 1234";
  int result_for_no_natural_number = maybe_take_natural_number(&str1);
  assert(result_for_no_natural_number == 0);
  assert(strcmp(str1, "foo bar 1234") == 0);

  char *str2 = "1234 foo bar";
  int result_for_natural_number = maybe_take_natural_number(&str2);
  assert(result_for_natural_number == 1234);
  assert(strcmp(str2, " foo bar") == 0);
}

void test_take_colour(void) {
  char *str1 = "red foo bar";
  enum Colour colour1 = take_colour(&str1);
  assert(colour1 == RED);
  assert(strcmp(str1, " foo bar") == 0);

  char *str2 = "green foo bar";
  enum Colour colour2 = take_colour(&str2);
  assert(colour2 == GREEN);
  assert(strcmp(str2, " foo bar") == 0);

  char *str3 = "blue foo bar";
  enum Colour colour3 = take_colour(&str3);
  assert(colour3 == BLUE);
  assert(strcmp(str3, " foo bar") == 0);
}

void test_maybe_take_round(void) {
  char *str = "6 green, 3 blue; 3 red, 1 green; 4 green, 3 red, 5 blue";
  Round *round1 = maybe_take_round(&str);
  assert(round1->red == 0);
  assert(round1->green == 6);
  assert(round1->blue == 3);
  assert(strcmp(str, "3 red, 1 green; 4 green, 3 red, 5 blue") == 0);

  Round *round2 = maybe_take_round(&str);
  assert(round2->red == 3);
  assert(round2->green == 1);
  assert(round2->blue == 0);
  assert(strcmp(str, "4 green, 3 red, 5 blue") == 0);

  Round *round3 = maybe_take_round(&str);
  assert(round3->red == 3);
  assert(round3->green == 4);
  assert(round3->blue == 5);
  assert(strcmp(str, "") == 0);

  Round *round4 = maybe_take_round(&str);
  assert(round4 == NULL);
  assert(strcmp(str, "") == 0);
}

int main(void) {
  test_take_word();
  test_maybe_take_word();
  test_take_colour();
  test_take_natural_number();
  test_take_header_returning_game_id();
  test_maybe_take_natural_number();
  test_maybe_take_round();
}