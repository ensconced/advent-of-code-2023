#include <stddef.h>

char *read_text_file(char *path);

typedef struct FileLines {
  char **lines;
  size_t line_count;
} FileLines;

FileLines read_file_lines(char *path);
