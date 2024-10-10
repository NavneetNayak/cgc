#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef struct block {
  size_t size;
  struct block *next;
  struct block *prev;
  int marked;
} block_t;

typedef struct alloc {
  block_t *free_list;
  block_t *used_list;

  void *bos;

  size_t allocated_size;
} alloc_t;

#define PAGE_SIZE 4096
#define HEADER_SIZE sizeof(block_t)
#define ALLOC_LIMIT 2 * PAGE_SIZE
#define ALLOC_FAILURE 1
#define ALLOC_SUCCESS 0

u_int16_t mem_dealloc(alloc_t *allocator, void *mem);
u_int16_t add_to_free_list(alloc_t *allocator, block_t *block);
void print_freelist(block_t *);
void print_usedlist(block_t *);
static uintptr_t *getbos();
alloc_t *alloc_init();
void *mem_alloc(alloc_t *allocator, size_t num_units);
void gc_collect(alloc_t *allocator);

