#include <stdio.h>
#include <stdlib.h>

long get_file_size(FILE *file) {
  long originalPosition = ftell(file);
  if (fseek(file, 0, SEEK_END) != 0) {
    printf("failed to seek to end of file\n");
    exit(1);
  }

  long result = ftell(file);

  if (fseek(file, originalPosition, SEEK_SET) != 0) {
    printf("failed to seek to original position of file\n");
    exit(1);
  }
  return result;
}

char *read_text_file(char *path) {
  FILE *file = fopen(path, "rt");
  if (file == NULL) {
    printf("file could not be opened\n");
    exit(1);
  }
  unsigned long fileSize = (unsigned long)get_file_size(file);
  char *buffer = malloc(fileSize + 1);
  if (buffer == NULL) {
    printf("failed to allocate\n");
    exit(1);
  }
  size_t readChars = fread(buffer, 1, fileSize, file);
  if (readChars != fileSize) {
    printf("didn't read as many chars as expected\n");
    exit(1);
  }
  buffer[fileSize] = '\0';

  if (fclose(file) != 0) {
    printf("failed to close file\n");
    exit(1);
  }

  return buffer;
}

typedef struct FileLines {
  char **lines;
  size_t line_count;
} FileLines;

size_t initial_lines_capacity = 2;

FileLines read_file_lines(char *path) {
  FILE *file = fopen(path, "rt");
  if (file == NULL) {
    printf("file could not be opened\n");
    exit(1);
  }

  char **lines = malloc(initial_lines_capacity * sizeof(char *));
  if (lines == NULL) {
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
        exit(1);
      }
      return (FileLines){.lines = lines, .line_count = lines_len};
    } else {
      lines[lines_len] = line_buffer;
      lines_len++;
    }
  }
}
