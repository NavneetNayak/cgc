#include "gc.h"

void test_allocator() {
  alloc_t *allocator = alloc_init();

  printf("Initial free list:\n");
  print_freelist(allocator->free_list);

  printf("Initial used list:\n");
  print_usedlist(allocator->used_list);

  void *mem1 = mem_alloc(allocator, 100);
  printf("After allocating 100 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  void *mem4 = mem_alloc(allocator, 9000);
  printf("After allocating 9000 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  void *mem2 = mem_alloc(allocator, 200);
  printf("After allocating 200 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem_dealloc(allocator, mem1);
  printf("After freeing 100 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem_dealloc(allocator, mem2);
  printf("After freeing 200 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);

  mem_dealloc(allocator, mem4);
  printf("After freeing 9000 bytes:\n");
  print_freelist(allocator->free_list);
  print_usedlist(allocator->used_list);
}

int main() {
  test_allocator();
  return 0;
}

