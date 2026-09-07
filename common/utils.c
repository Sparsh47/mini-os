#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **parser(char *input, char *ARGS[]) {
  int i = 0;

  char *token = strtok(input, " \t");

  while (token != NULL) {
    ARGS[i] = token;
    i++;
    token = strtok(NULL, " \t");
  }

  ARGS[i] = NULL;
  return ARGS;
}
