#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS 64

int main() {
  char *input = NULL;
  size_t size = 0;
  printf("$> ");

  while (1) {
    getline(&input, &size, stdin);

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      free(input);
      break;
    }

    pid_t pid = fork();

    if (pid < 0) {
      perror("fork");
      break;
    } else if (pid == 0) {
      char **args = malloc(sizeof(char *) * MAX_ARGS);
      parser(input, args);
      if (args[0] != NULL) {
        execvp(args[0], args);
        perror("execvp");
      }
      free(args);
      exit(1);
    } else {
      waitpid(pid, NULL, 0);
    }
    printf("$> ");
  }

  free(input);
  return 0;
}
