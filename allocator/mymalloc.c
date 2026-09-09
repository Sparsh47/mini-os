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

  header->size = size;
  header->magic = MAGIC;
  header->status = ALLOCATED;

  void *user_ptr = (void *)((char *)header + sizeof(struct mem_header));

  return user_ptr;
}

void my_free(void *ptr) {
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

  int *p = my_malloc(sizeof(int));

  *p = 42;

  printf("Value of p: %d\n", *p);

  printf("Address of p: %p\n", (void *)p);

  return 0;
}