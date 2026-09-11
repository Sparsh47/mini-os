#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "policies.h"

char mem_heap[HEAP_SIZE];
struct mem_header *allocator_ptr = NULL;

void init_heap(void) {
  struct mem_header *header = (struct mem_header *)mem_heap;

  header->size = HEAP_SIZE - sizeof(struct mem_header);
  header->magic = MAGIC;
  header->status = FREE;
}

void *my_malloc(size_t size) {
  struct mem_header *header = first_fit_search(size);
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

  printf("\nState of free list after allocating a, b, c:\n");
  print_free_list();

  // 2. Free 'b'
  printf("\nFreeing b...\n");
  my_free(b);
  print_free_list();

  // 3. Allocate 'd' — should reuse the slot freed by 'b'!
  int *d = my_malloc(sizeof(int));
  *d = 99;
  printf("d: %d at %p (reused b's slot? %s)\n", *d, (void *)d,
         (d == b) ? "YES!" : "NO");

  // 4. Test Coalescing: Free 'd' and 'c' which are adjacent
  printf("\nFreeing d and c...\n");
  my_free(d);
  my_free(c);
  print_free_list();

  // Allocate a chunk larger than single slots to confirm merged contiguous space
  int *large = my_malloc(sizeof(int) * 10);
  printf("large: allocated at %p (reused coalesced block starting at b? %s)\n",
         (void *)large, (large == (int *)b) ? "YES!" : "NO");

  my_free(a);
  my_free(large);

  return 0;
}
