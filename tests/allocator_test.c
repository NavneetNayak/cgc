#include "allocator.h"

void print_freelist(block_t *list) {
  printf("Free List: \n");
  block_t *current = list->next;
  do {
    printf("Block at %p: size=%zu, marked=%d\n", (void *)current, current->size,
           current->marked);
    current = current->next;
  } while (current != NULL && current != list);
  printf("\n");
}

void print_usedlist(block_t *list) {
  printf("Used List: \n");
  while (list != NULL) {
    printf("Block at %p: size=%zu, marked=%d\n", (void *)list, list->size,
           list->marked);
    list = list->next;
  }
  printf("\n");
}

void test_allocator() {
  alloc_init();

  printf("Initial free list:\n");
  print_freelist(allocator.free_list);

  printf("Initial used list:\n");
  print_usedlist(allocator.used_list);

  void *mem1 = mem_alloc(100);
  printf("After allocating 100 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  void *mem4 = mem_alloc(9000);
  printf("After allocating 9000 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  void *mem2 = mem_alloc(200);
  printf("After allocating 200 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  mem_dealloc(mem1);
  printf("After freeing 100 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  mem_dealloc(mem2);
  printf("After freeing 200 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  mem_dealloc(mem4);
  printf("After freeing 9000 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);
}

int main() {
  test_allocator();
  return 0;
}
