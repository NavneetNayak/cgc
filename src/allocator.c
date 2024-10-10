#include "gc.h"

static void *mempage_fetch(unsigned int num_pages) {
  void *block = mmap(NULL, num_pages * PAGE_SIZE, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (block == MAP_FAILED) {
    return NULL;
  }

  return block;
}

static void *mem_fetch(unsigned int num_bytes) {
  void *mem = mmap(NULL, num_bytes, PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (mem == MAP_FAILED) {
    return NULL;
  }

  return mem;
}

alloc_t *alloc_init() {
  alloc_t *allocator = (alloc_t *)malloc( sizeof(alloc_t) );
  if (allocator == NULL) {
    return NULL;
  }

  if ((allocator->free_list = mem_fetch(HEADER_SIZE)) == NULL) {
    return NULL;
  }
  allocator->free_list->marked = 1;
  allocator->free_list->next = allocator->free_list;
  allocator->free_list->prev = allocator->free_list;
  allocator->free_list->size = 0;

  if ((allocator->used_list = mem_fetch(HEADER_SIZE)) == NULL) {
    return NULL;
  }
  allocator->used_list->marked = 1;
  allocator->used_list->next = NULL;
  allocator->used_list->prev = NULL;
  allocator->used_list->size = 0;

  return allocator;
}

u_int16_t add_to_free_list(alloc_t *allocator, block_t *block) {
  block_t *free_list = allocator->free_list;

  while (!(block > free_list && block < free_list->next)) {
    if (free_list >= free_list->next &&
        (block > free_list || block < free_list->next)) {
      break;
    }
    free_list = free_list->next;
  }

  if ((char *)block + block->size == (char *)free_list->next) {
    block->size += free_list->next->size;
    block->next = free_list->next->next;
    if (free_list->next->next != NULL) {
      free_list->next->next->prev = block;
    }
  } else {
    block->next = free_list->next;
    free_list->next->prev = block;
  }

  if ((char *)free_list + free_list->size == (char *)block) {
    free_list->size += block->size;
    free_list->next = block->next;
    if (block->next != NULL) {
      block->next->prev = free_list;
    }
  } else {
    free_list->next = block;
  }

  block->prev = free_list;
  return ALLOC_SUCCESS;
}

void *mem_alloc(alloc_t *allocator, size_t num_units) {
  if (allocator->allocated_size > ALLOC_LIMIT) {
    gc_collect(allocator);
  }

  size_t total_size = (num_units + sizeof(block_t) - 1) / sizeof(block_t) + 1;
  total_size *= sizeof(block_t);

  block_t *free_list = allocator->free_list;

  do {
    if (free_list->size >= total_size) {
      if (free_list->size == total_size) {
        free_list->prev->next = free_list->next;
        if (free_list->next != NULL) {
          free_list->next->prev = free_list->prev;
        }

        free_list->next = allocator->used_list->next;
        if (allocator->used_list->next != NULL) {
          allocator->used_list->next->prev = free_list;
        }
        free_list->prev = allocator->used_list;
        allocator->used_list->next = free_list;

        allocator->allocated_size += free_list->size;
        return (void *)(free_list + 1);
      }
      if (free_list->size - total_size >= sizeof(block_t)) {
        block_t *new_block = (block_t *)((char *)free_list + total_size);
        new_block->size = free_list->size - total_size;
        new_block->next = free_list->next;
        new_block->prev = free_list->prev;
        free_list->prev->next = new_block;
        if (free_list->next != NULL) {
          free_list->next->prev = new_block;
        }

        free_list->size = total_size;
        free_list->next = allocator->used_list->next;
        if (allocator->used_list->next != NULL) {
          allocator->used_list->next->prev = free_list;
        }
        free_list->prev = allocator->used_list;
        allocator->used_list->next = free_list;

        allocator->allocated_size += free_list->size;
        return (void *)(free_list + 1);
      }
    }
    free_list = free_list->next;
  } while (free_list != allocator->free_list);

  size_t total_pages = (total_size + PAGE_SIZE - 1) / PAGE_SIZE;
  block_t *new_block = mempage_fetch(total_pages);
  if (new_block == NULL) {
    return NULL;
  }

  new_block->next = NULL;
  new_block->prev = NULL;
  new_block->size = total_pages * PAGE_SIZE;

  add_to_free_list(allocator, new_block);
  return mem_alloc(allocator, num_units);
}

u_int16_t mem_dealloc(alloc_t *allocator, void *mem) {
  block_t *block = (block_t *)mem - 1;

  // Remove the block from the use list
  if (block->prev != NULL) {
    block->prev->next = block->next;
  }
  if (block->next != NULL) {
    block->next->prev = block->prev;
  }

  // add the block back to the free list
  if (add_to_free_list(allocator, block) == ALLOC_FAILURE) {
    return ALLOC_FAILURE;
  }

  return ALLOC_SUCCESS;
}

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

