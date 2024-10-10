#include "gc.h"

typedef struct ts {
  char *random1;
  int random2;
  struct ts *pointer;
} test_struct;

void test_gc (alloc_t *allocator) {
  printf("Initial free list: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);

  printf("Initial used list: Allocated size: %ld\n", allocator->allocated_size);
  print_usedlist(allocator->used_list);

  void *mem1 = mem_alloc(allocator, 100);
  printf("***After allocating 100 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  void *mem4 = mem_alloc(allocator, 9000);
  printf("***After allocating 9000 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  void *mem2 = mem_alloc(allocator, 200);
  printf("***After allocating 200 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem4 = mem_alloc(allocator, 1200);
  printf("***After allocating 1200 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  void *mem5 = mem4;

  // Create unusable memory
  mem4 = mem_alloc(allocator, 300);
  printf("***After allocating 300 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem4 = mem_alloc(allocator, 600);
  printf("***After allocating 600 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem4 = mem_alloc(allocator, 900);
  printf("***After allocating 900 bytes: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  test_struct *teststruct = (test_struct *)mem_alloc(allocator, sizeof(test_struct));
  printf("***After allocating %ld bytes: Allocated size: %ld\n", sizeof(test_struct), allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  teststruct->pointer = (test_struct *)mem_alloc(allocator, sizeof(test_struct));
  printf("***After allocating %ld bytes: Allocated size: %ld\n", sizeof(test_struct), allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  teststruct->pointer = teststruct;

  gc_collect(allocator);
  printf("***After garbage collecting: Allocated size: %ld\n", allocator->allocated_size);
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);
}

int main (int argc, char **argv) {
	alloc_t *allocator = alloc_init();
	allocator->bos = &argc;
	test_gc(allocator);
	return 0;
}

