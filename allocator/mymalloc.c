#include <stdio.h>
#include <stdlib.h>

#define HEAP_SIZE 4096
#define MAGIC 0xDEADBEEF

static char mem_heap[HEAP_SIZE];

enum allocation_status { FREE, ALLOCATED };

struct mem_header {
  size_t size;
  int magic;
  enum allocation_status status;
};

void *first_fit_search(size_t size) {
  char *curr = mem_heap;

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *header = (struct mem_header *)curr;

    if (header->magic != MAGIC) {
      break;
    }

    if (header->status == FREE && header->size >= size) {
      return header;
    }

    curr += sizeof(struct mem_header) + header->size;
  }

  return NULL;
}

void init_heap() {
  struct mem_header *header = (struct mem_header *)mem_heap;

  header->size = HEAP_SIZE - sizeof(struct mem_header);
  header->magic = MAGIC;
  header->status = FREE;
}

void *my_malloc(size_t size) {

  struct mem_header *header = first_fit_search(size);
  if (header == NULL) {
    fprintf(stderr, "No memory left ");
    return NULL;
  }

  if (header->size >= size + sizeof(struct mem_header) + 1) {
    size_t remaining_size = header->size - size - sizeof(struct mem_header);
    struct mem_header *next_header =
        (struct mem_header *)((char *)header + sizeof(struct mem_header) +
                              size);
    next_header->size = remaining_size;
    next_header->magic = MAGIC;
    next_header->status = FREE;
    header->size = size;
  }

  header->status = ALLOCATED;

  void *user_ptr = (void *)((char *)header + sizeof(struct mem_header));

  return user_ptr;
}

void my_free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  struct mem_header *header =
      (struct mem_header *)((char *)ptr - sizeof(struct mem_header));

  if (header->magic != MAGIC) {
    fprintf(stderr, "Segmentation Fault");
    return;
  }

  header->status = FREE;
}

int main() {
  init_heap();

  // 1. Allocate three separate integers
  int *a = my_malloc(sizeof(int));
  int *b = my_malloc(sizeof(int));
  int *c = my_malloc(sizeof(int));

  *a = 10;
  *b = 20;
  *c = 30;

  printf("a: %d at %p\n", *a, (void *)a);
  printf("b: %d at %p\n", *b, (void *)b);
  printf("c: %d at %p\n", *c, (void *)c);

  // 2. Free 'b'
  printf("\nFreeing b...\n");
  my_free(b);

  // 3. Allocate 'd' — should reuse the slot freed by 'b'!
  int *d = my_malloc(sizeof(int));
  *d = 99;
  printf("d: %d at %p (reused b's slot? %s)\n", *d, (void *)d,
         (d == b) ? "YES!" : "NO");

  return 0;
}
