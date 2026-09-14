#include <stdio.h>
#include <stdlib.h>

#define VPN_MASK 240
#define OFFSET_MASK 15

#define PAGE_TABLE_SIZE 16

struct page_table_entry {
  int vpn;
  int pfn;
  int valid;
  char permissions[4];
};

struct page_table_entry page_table[PAGE_TABLE_SIZE];

void init_page_table(void) {
  srand(42);

  for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
    page_table[i].vpn = i;

    page_table[i].valid = (rand() % 100 < 75) ? 1 : 0;

    if (page_table[i].valid) {
      page_table[i].pfn = rand() % 64;

      page_table[i].permissions[0] = 'r';
      page_table[i].permissions[1] = (rand() % 2) ? 'w' : '-';
      page_table[i].permissions[2] = (rand() % 3 == 0) ? 'x' : '-';
    } else {
      page_table[i].pfn = -1;
      page_table[i].permissions[0] = '-';
      page_table[i].permissions[1] = '-';
      page_table[i].permissions[2] = '-';
    }
    page_table[i].permissions[3] = '\0';
  }
}

void translate_address(char *binary_address) {

  char *endptr;
  long val = strtol(binary_address, &endptr, 2);

  int vpn = (val & VPN_MASK) >> 4;
  int offset = val & OFFSET_MASK;

  struct page_table_entry pte = page_table[vpn];

  if (pte.valid == 0) {
    printf("Page fault in page %d\n", vpn);
    printf("======================================\n");
    return;
  }

  if (pte.permissions[0] != 'r') {
    printf("Protection fault in page %d\n", vpn);
    printf("======================================\n");
    return;
  }

  int pfn = pte.pfn;

  int physical_address = (pfn << 4) | offset;

  printf("%s (%d) (vpn : %d) (offset : %d) (pfn : %d)\n", binary_address,
         physical_address, vpn, offset, pfn);
  printf("======================================\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <8-bit binary address (e.g. 11010101)>\n",
            argv[0]);
    return 1;
  }

  init_page_table();

  printf("%-10s %-10s %-10s %-10s\n", "VPN", "PFN", "Valid", "Permissions");
  for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
    printf("%-10d %-10d %-10d %-10s\n", page_table[i].vpn, page_table[i].pfn,
           page_table[i].valid, page_table[i].permissions);
  }

  char line[10];

  FILE *file = fopen(argv[1], "r");

  if (file == NULL) {
    fprintf(stderr, "Error opening file\n");
    return 1;
  }

  while (fgets(line, sizeof(line), file) != NULL) {
    translate_address(line);
  }

  fclose(file);

  return 0;
}