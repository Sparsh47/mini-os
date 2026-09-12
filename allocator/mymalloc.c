#include "mymalloc.h"
#include "benchmark.h"
#include "policies.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
  POLICY_FIRST_FIT,
  POLICY_BEST_FIT,
  POLICY_WORST_FIT,
  POLICY_NEXT_FIT
} alloc_policy_t;

#define ALLOC_POLICY POLICY_NEXT_FIT

char mem_heap[HEAP_SIZE];
struct mem_header *allocator_ptr = NULL;

void init_heap(void) {
  struct mem_header *header = (struct mem_header *)mem_heap;

  header->size = HEAP_SIZE - sizeof(struct mem_header);
  header->magic = MAGIC;
  header->status = FREE;
}

void *my_malloc(size_t size) {
  struct mem_header *header = NULL;
  switch (ALLOC_POLICY) {
  case POLICY_FIRST_FIT:
    header = first_fit_search(size);
    break;
  case POLICY_WORST_FIT:
    header = worst_fit_search(size);
    break;
  case POLICY_BEST_FIT:
    header = best_fit_search(size);
    break;
  case POLICY_NEXT_FIT:
    header = next_fit_search(size);
    break;
  }

  if (header == NULL) {
    fprintf(stderr, "No memory left\n");
    return NULL;
  }

  split_block(header, size);

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
    fprintf(stderr, "Segmentation Fault\n");
    return;
  }

  header->status = FREE;
  coalesce();
}

int main(void) {
  init_heap();

  printf("\n=== Running Fragmentation Benchmark ===\n");
  switch (ALLOC_POLICY) {
  case POLICY_FIRST_FIT:
    printf("First Fit\n");
    break;
  case POLICY_WORST_FIT:
    printf("Worst Fit\n");
    break;
  case POLICY_BEST_FIT:
    printf("Best Fit\n");
    break;
  case POLICY_NEXT_FIT:
    printf("Next Fit\n");
    break;
  }
  fragmentation_benchmark();

  return 0;
}
