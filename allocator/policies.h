#ifndef POLICIES_H
#define POLICIES_H

#include <stddef.h>
#include "mymalloc.h"

// Search policies
struct mem_header *first_fit_search(size_t size);
struct mem_header *worst_fit_search(size_t size);
struct mem_header *best_fit_search(size_t size);
struct mem_header *next_fit_search(size_t size);

// Splitting policy
void split_block(struct mem_header *header, size_t size);

// Coalescing policies
void coalesce(void);
void coalesce_blocks(void);

// Inspection / Debugging
void print_free_list(void);

#endif // POLICIES_H
