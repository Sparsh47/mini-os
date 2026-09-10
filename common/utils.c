#include <ctype.h>
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

void remove_spaces(char *input) {
  int i = 0, j = 0;

  while (input[i] != '\0') {
    if (!isspace((unsigned char)input[i])) {
      input[j++] = input[i];
    }
    i++;
  }

  input[j] = '\0';
}