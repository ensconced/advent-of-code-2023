#include "./lib.h"
#include <stdio.h>
#include <stdlib.h>

long get_file_size(FILE *file) {
  long originalPosition = ftell(file);
  if (fseek(file, 0, SEEK_END) != 0) {
    printf("failed to seek to end of file\n");
    exit(EXIT_FAILURE);
  }

  long result = ftell(file);

  if (fseek(file, originalPosition, SEEK_SET) != 0) {
    printf("failed to seek to original position of file\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

char *read_text_file(char *path) {
  FILE *file = fopen(path, "rt");
  if (file == 0) {
    printf("file could not be opened\n");
    exit(EXIT_FAILURE);
  }
  unsigned long fileSize = (unsigned long)get_file_size(file);
  char *buffer = malloc(fileSize + 1);
  if (buffer == 0) {
    printf("failed to allocate\n");
    exit(EXIT_FAILURE);
  }
  size_t readChars = fread(buffer, 1, fileSize, file);
  if (readChars != fileSize) {
    printf("didn't read as many chars as expected\n");
    exit(EXIT_FAILURE);
  }
  buffer[fileSize] = '\0';

  if (fclose(file) != 0) {
    printf("failed to close file\n");
    exit(EXIT_FAILURE);
  }

  return buffer;
}

size_t initial_lines_capacity = 2;

FileLines read_file_lines(char *path) {
  FILE *file = fopen(path, "rt");
  if (file == 0) {
    printf("file could not be opened\n");
    exit(EXIT_FAILURE);
  }

  char **lines = malloc(initial_lines_capacity * sizeof(char *));
  if (lines == 0) {
    printf("failed initial allocation of lines\n");
  }

  size_t lines_capacity = initial_lines_capacity;
  size_t lines_len = 0;

  while (1) {
    size_t line_buffer_capacity = 2;
    char *line_buffer = malloc(line_buffer_capacity);
    ssize_t result = getline(&line_buffer, &line_buffer_capacity, file);

    if (lines_len + 1 > lines_capacity) {
      lines_capacity *= 2;
      lines = realloc(lines, lines_capacity * sizeof(char *));
    }

    if (result == -1) {
      if (fclose(file) != 0) {
        printf("failed to close file\n");
        exit(EXIT_FAILURE);
      }
      return (FileLines){.lines = lines, .line_count = lines_len};
    } else {
      lines[lines_len] = line_buffer;
      lines_len++;
    }
  }
}

void free_file_lines(FileLines file_lines) {
  for (size_t i = 0; i < file_lines.line_count; ++i) {
    free(file_lines.lines[i]);
  }
  free(file_lines.lines);
}
