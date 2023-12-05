#include "./lib.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_read_text_file(void) {
  char *expected_value = "hello there";
  char *value_from_file = read_text_file("./example.txt");
  assert(strcmp(expected_value, value_from_file) == 0);
  free(value_from_file);
}
void test_read_file_lines(void) {
  char *expected_lines[] = {
      "hey\n", "there\n", "here\n", "are\n",  "some\n", "lines\n",
      "I\n",   "hope\n",  "you\n",  "like\n", "them",
  };
  size_t expected_line_count = 11;
  FileLines file_lines = read_file_lines("./example_lines.txt");
  assert(file_lines.line_count == expected_line_count);
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    assert(strcmp(file_lines.lines[i], expected_lines[i]) == 0);
  }
  free_file_lines(file_lines);
}

int main(void) {
  test_read_text_file();
  test_read_file_lines();
}
