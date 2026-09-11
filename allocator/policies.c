#include "policies.h"
#include "mymalloc.h"
#include <stdio.h>
#include <stdlib.h>

// Search Policies

struct mem_header *first_fit_search(size_t size) {
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

struct mem_header *worst_fit_search(size_t size) {
  char *curr = mem_heap;
  struct mem_header *ans_ptr = NULL;

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *header = (struct mem_header *)curr;

    if (header->magic != MAGIC) {
      break;
    }

    if (header->status == FREE && header->size >= size) {
      if (ans_ptr == NULL) {
        ans_ptr = header;
      } else {
        if (ans_ptr->size < header->size) {
          ans_ptr = header;
        }
      }
    }

    curr += sizeof(struct mem_header) + header->size;
  }

  return ans_ptr;
}

struct mem_header *best_fit_search(size_t size) {
  char *curr = mem_heap;
  struct mem_header *ans_ptr = NULL;

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *header = (struct mem_header *)curr;

    if (header->magic != MAGIC) {
      break;
    }

    if (header->status == FREE && header->size >= size) {
      if (ans_ptr == NULL) {
        ans_ptr = header;
      } else {
        if (ans_ptr->size > header->size) {
          ans_ptr = header;
        }
      }
    }

    curr += sizeof(struct mem_header) + header->size;
  }

  return ans_ptr;
}

struct mem_header *next_fit_search(size_t size) {
  char *curr = (char *)allocator_ptr;

  if (curr == NULL) {
    curr = mem_heap;
  }

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *header = (struct mem_header *)curr;

    if (header->magic != MAGIC) {
      break;
    }

    if (header->status == FREE && header->size >= size) {
      allocator_ptr = header;
      return header;
    }

    curr += sizeof(struct mem_header) + header->size;

    if (curr == mem_heap + HEAP_SIZE) {
      curr = mem_heap;
    }

    if ((struct mem_header *)curr == allocator_ptr) {
      break;
    }
  }

  return NULL;
}

// Splitting Policy

void split_block(struct mem_header *header, size_t size) {
  if (header == NULL) {
    return;
  }

  if (header->size < size + sizeof(struct mem_header)) {
    fprintf(stderr, "Not enough memory.\n");
    return;
  }

  size_t remaining_size = header->size - size - sizeof(struct mem_header);

  header->size = size;

  struct mem_header *next_header =
      (struct mem_header *)((char *)header + sizeof(struct mem_header) + size);

  next_header->size = remaining_size;
  next_header->magic = MAGIC;
  next_header->status = FREE;

  return;
}

// Coalescing Policy

void coalesce(void) {}

void coalesce_blocks(void) {}

// Free List Inspection

void print_free_list(void) {
  printf("--- Free List ---\n");
  char *curr = mem_heap;
  int count = 0;

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *header = (struct mem_header *)curr;

    if (header->magic != MAGIC) {
      break;
    }

    if (header->status == FREE) {
      printf("[Free Block %d] Header: %p, Payload: %p, Size: %zu bytes\n",
             count, (void *)header,
             (void *)((char *)header + sizeof(struct mem_header)),
             header->size);
      count++;
    }

    curr += sizeof(struct mem_header) + header->size;
  }

  if (count == 0) {
    printf("(No free blocks)\n");
  }
  printf("Total free blocks: %d\n", count);
  printf("-----------------\n");
}
