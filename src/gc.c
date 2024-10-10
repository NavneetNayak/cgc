#include "gc.h"

static void scan_mem(alloc_t *allocator, uintptr_t *start, uintptr_t *end) {
  block_t *current_block = allocator->used_list->next;

  // Traverse through each pointer in the region defined by start and end
  for (; start < end; start++) {
    uintptr_t ptr_value = *start;

    // Check if the pointer value falls within any allocated block
    while (current_block != NULL) {
      if (current_block->marked) {
        current_block = current_block->next;
        continue;
      }

      uintptr_t block_start = (uintptr_t)current_block;
      uintptr_t block_end = block_start + 1 + current_block->size;

      if (ptr_value >= block_start && ptr_value < block_end) {
        // printf("Marked block of size: %ld\n", current_block->size);
        current_block->marked = 1; // Mark the block as reachable
        break;
      }

      current_block = current_block->next;
    }

    // Reset to the start of the used list for next pointer in the region
    current_block = allocator->used_list->next;
  }
}

void gc_collect(alloc_t *allocator) {
  printf("***Starting Garbage Collection Process*** Allocation size: %ld\n", allocator->allocated_size);

  void *bos = allocator->bos;
  void *tos = __builtin_frame_address(0);

  scan_mem(allocator, tos, bos);
  // print_usedlist(allocator->used_list);

  // printf("Finished scanning root pointers\n");

  // DFS
  block_t *used_list = allocator->used_list->next;
  while (used_list != NULL) {
    if (used_list->marked == 0) {
      used_list = used_list->next;
      continue;
    }

    // printf("Currently scanning block of size: %ld\n", used_list->size);
    scan_mem(allocator, (void *)(used_list + 1), (void *)(used_list) + used_list->size);

    used_list = used_list->next;
  }

  // print_usedlist(allocator->used_list);

  block_t *current = allocator->used_list->next;
  while (current != NULL) {
    if (current->marked) {
      current->marked = 0; // Reset mark for next collection
      current = current->next;
    } else {
      // Block is not marked (unreachable), free it
      block_t *unused_block = current;
      current = current->next;

      // Remove from used list
      if (unused_block->prev != NULL) {
        unused_block->prev->next = unused_block->next;
      }
      if (unused_block->next != NULL) {
        unused_block->next->prev = unused_block->prev;
      }

      // Add to free list
      // printf("Reclaimed block of size: %ld\n", unused_block->size);
      size_t size = unused_block->size;
      if ( add_to_free_list(allocator, unused_block) != ALLOC_FAILURE) {
        allocator->allocated_size -= size;
      }
    }
  }
}

