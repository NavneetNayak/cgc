typedef struct ts {
  char *random1;
  int random2;
  struct ts *pointer;
} test_struct;

typedef struct obj {
  int num;
} Obj;

static Obj **return_pointer_array() {
  printf("%ld\n", sizeof(Obj));
  Obj **obj_array = (Obj **)mem_alloc(100 * sizeof(Obj));
  printf("After allocating obj array %ld\n", 100 * sizeof(Obj));
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  int i = 0;
  int index = 0;

  while (i < 100) {
    Obj *new_obj = (Obj *)mem_alloc(sizeof(Obj));
    new_obj->num = i;
    if (i % 10 == 0) {
      obj_array[index++] = new_obj;
    }
    i++;
  }
  return obj_array;
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

  mem4 = mem_alloc(1200);
  printf("After allocating 300 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  void *mem5 = mem4;

  // Create unusable memory
  mem4 = mem_alloc(300);
  printf("After allocating 300 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  mem4 = mem_alloc(600);
  printf("After allocating 600 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  mem4 = mem_alloc(900);
  printf("After allocating 900 bytes:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  test_struct *teststruct = (test_struct *)mem_alloc(sizeof(test_struct));
  printf("After allocating %ld bytes:\n", sizeof(test_struct));
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  teststruct->pointer = (test_struct *)mem_alloc(sizeof(test_struct));
  printf("After allocating %ld bytes:\n", sizeof(test_struct));
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);

  teststruct->pointer->pointer = teststruct;

  // mem1 = NULL;
  // mem2 = NULL;
  // Make teststruct point somewhere else
  // teststruct->pointer = NULL;
  // mem4 = NULL;
  // mem5 = NULL;
  // teststruct->pointer = NULL;
  // teststruct = NULL;

  // Obj **obj_array = return_pointer_array();

  gc_collect();
  printf("After garbage collecting:\n");
  print_freelist(allocator.free_list);
  print_usedlist(allocator.used_list);
}
