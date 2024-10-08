// #define _XOPEN_SOURCE 700
// #define _DARWIN_C_SOURCE

// #include <dlfcn.h>
// #include <execinfo.h> // for backtrace functions
// #include <libunwind.h>
// #include <pthread.h>
// #include <stdbool.h>
// #include <stddef.h>
// #include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <sys/_types/_u_int32_t.h>
// #include <sys/_types/_u_int8_t.h>
#include <sys/mman.h>
// #include <sys/sysctl.h>
// #include <ucontext.h>
// #include <unistd.h>

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
} alloc_t;

#define PAGE_SIZE 4096
#define HEADER_SIZE sizeof(block_t)
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
