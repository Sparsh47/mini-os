#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }
  char path[256];
  snprintf(path, sizeof(path), "/proc/%s/maps", argv[1]);

  FILE *file;

  file = fopen(path, "r");

  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  char line[1024];
  char address[256];
  char permissions[256];
  char offset[256];
  char device[256];
  char inode[256];
  char pathname[256];
  char *derived_region = "";

  printf("\n%-25s || %-12s || %-20s || %-15s || %-10s || %-50s || %s\n",
         "START-END", "PERMISSIONS", "OFFSET", "DEVICE", "INODE", "PATHNAME",
         "REGION TYPE");
  while (fgets(line, sizeof(line), file)) {
    pathname[0] = '\0';
    sscanf(line, "%s %s %s %s %s %s", address, permissions, offset, device,
           inode, pathname);

    if (strcmp(pathname, "[heap]") == 0) {
      derived_region = "HEAP";
    } else if (strcmp(pathname, "[stack]") == 0) {
      derived_region = "STACK";
    } else if (strcmp(pathname, "[vvar]") == 0) {
      derived_region = "VVAR";
    } else if (strcmp(pathname, "[vdso]") == 0) {
      derived_region = "VDSO";
    } else if (strcmp(pathname, "[vsyscall]") == 0 ||
               strcmp(pathname, "[vsys]") == 0) {
      derived_region = "VSYS";
    } else {
      if (strstr(pathname, ".so") != NULL && strstr(permissions, "x") != NULL) {
        derived_region = "CODE";
      } else {
        derived_region = "ANONYMOUS";
      }
    }

    printf("-------------------------------------------------------------------"
           "-------------------------------------------------------------------"
           "-------------------------------------------------------------------"
           "\n");
    printf("%-25s || %-12s || %-20s || %-15s || %-10s || %-50s || %s\n",
           address, permissions, offset, device, inode, pathname,
           derived_region);
  }

  fclose(file);

  return 0;
}