#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stddef.h>

#define HEAP_SIZE 4096
#define MAGIC 0xDEADBEEFU

enum allocation_status { FREE, ALLOCATED };

struct mem_header {
  size_t size;
  unsigned int magic;
  enum allocation_status status;
};

extern char mem_heap[HEAP_SIZE];
extern struct mem_header *allocator_ptr;

void init_heap(void);
void *my_malloc(size_t size);
void my_free(void *ptr);
void print_free_list(void);

#endif
