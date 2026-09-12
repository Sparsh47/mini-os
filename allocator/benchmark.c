#include "benchmark.h"
#include "mymalloc.h"
#include "policies.h"
#include <stdio.h>
#include <stdlib.h>

#define NUM_SLOTS 64
#define TOTAL_OPS 150

// Calculate external fragmentation: 1 - (largest_free / total_free)
static void calculate_fragmentation(double *frag_out, size_t *total_free_out,
                                    size_t *max_free_out,
                                    int *total_free_blocks,
                                    int *total_blocks_out) {
  size_t total_free = 0;
  size_t max_free = 0;
  int free_count = 0;
  int total_count = 0;
  char *curr = mem_heap;

  while (curr < mem_heap + HEAP_SIZE) {
    struct mem_header *h = (struct mem_header *)curr;
    if (h->magic != MAGIC)
      break;

    total_count++;
    if (h->status == FREE) {
      total_free += h->size;
      if (h->size > max_free) {
        max_free = h->size;
      }
      free_count++;
    }
    curr += sizeof(struct mem_header) + h->size;
  }

  *total_free_out = total_free;
  *max_free_out = max_free;
  *total_free_blocks = free_count;
  *total_blocks_out = total_count;
  *frag_out =
      (total_free > 0) ? (1.0 - ((double)max_free / (double)total_free)) : 0.0;
}

void fragmentation_benchmark(void) {
  init_heap();

  void *ptrs[NUM_SLOTS] = {NULL};
  int failed_allocs = 0;
  int successful_allocs = 0;

  // Fixed seed for 100% reproducible benchmark across all policies
  srand(18);

  // Realistic sizes for a 4KB heap (16B to 128B)
  size_t test_sizes[] = {16, 24, 32, 48, 64, 96, 128};
  int num_sizes = sizeof(test_sizes) / sizeof(test_sizes[0]);

  for (int i = 0; i < TOTAL_OPS; i++) {
    int slot = rand() % NUM_SLOTS;

    if (ptrs[slot] != NULL) {
      // Slot is occupied: randomly decide to free (40% chance to free, keeps
      // memory pressure higher)
      if (rand() % 10 < 4) {
        my_free(ptrs[slot]);
        ptrs[slot] = NULL;
      }
    } else {
      // Slot is empty: allocate a random size
      size_t sz = test_sizes[rand() % num_sizes];
      ptrs[slot] = my_malloc(sz);

      if (ptrs[slot] == NULL) {
        failed_allocs++;
      } else {
        successful_allocs++;
      }
    }
  }

  // Measure final fragmentation state
  double fragmentation = 0.0;
  size_t total_free = 0;
  size_t max_free = 0;
  int total_free_blocks = 0;
  int total_blocks = 0;
  calculate_fragmentation(&fragmentation, &total_free, &max_free,
                          &total_free_blocks, &total_blocks);

  printf("Benchmark Results:\n");
  printf("  Successful Allocs : %d\n", successful_allocs);
  printf("  Failed Allocs     : %d\n", failed_allocs);
  printf("  Total Blocks      : %d (Free: %d, Allocated: %d)\n", total_blocks,
         total_free_blocks, total_blocks - total_free_blocks);
  printf("  Total Free Memory : %zu bytes\n", total_free);
  printf("  Largest Free Block: %zu bytes\n", max_free);
  printf("  Fragmentation     : %.2f%%\n", fragmentation * 100.0);
}
