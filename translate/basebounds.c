#include "../common/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void address_translation(int base, int bounds, int virtual_address) {
  if (virtual_address < 0 || virtual_address >= bounds) {
    printf("Virtual Address %d: FAULT\n", virtual_address);
    return;
  }

  printf("Physical Address for Virtual Address %d = %d\n", virtual_address,
         virtual_address + base);
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments");
    return 1;
  }

  if (strlen(argv[1]) > 0) {
    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
      fprintf(stderr, "Error opening file");
      return 1;
    }

    char line[256];

    while (fgets(line, sizeof(line), file) != NULL) {
      int base, bounds, virtual_address;
      int check = sscanf(line, "%d %d %d", &base, &bounds, &virtual_address);
      if (check == 3) {
        address_translation(base, bounds, virtual_address);
      }
    }

    fclose(file);
  }

  return 0;
}