#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SEGMENT_MASK 192
#define OFFSET_MASK 63

#define CODE_BASE 1000
#define CODE_BOUNDS 32

#define HEAP_BASE 4000
#define HEAP_BOUNDS 40

#define STACK_BASE 8000
#define STACK_BOUNDS 24

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    perror("Error Opening File");
    exit(1);
  }

  int segment, offset;

  char line[10];

  while (fgets(line, sizeof(line), file) != NULL) {
    char *endptr;

    long val = strtol(line, &endptr, 2);

    segment = (val & SEGMENT_MASK) >> 6;
    offset = val & OFFSET_MASK;

    switch (segment) {
    case 0:
      if (offset < CODE_BOUNDS) {
        printf("Physical Address = %d\n", CODE_BASE + offset);
      } else {
        printf("FAULT: %s\n", line);
      }
      break;
    case 1:
      if (offset < HEAP_BOUNDS) {
        printf("Physical Address = %d\n", HEAP_BASE + offset);
      } else {
        printf("FAULT: %s\n", line);
      }
      break;
    case 2:
      if (offset < STACK_BOUNDS) {
        printf("Physical Address = %d\n", STACK_BASE - offset);
      } else {
        printf("FAULT: %s\n", line);
      }
      break;
    default:
      printf("FAULT: %s\n", line);
      break;
    }
  }

  fclose(file);

  return 0;
}