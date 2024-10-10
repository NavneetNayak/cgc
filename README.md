# cgc
A mark and sweep garbage collector written in c

## Usage
```
#include "gc.h"

------------------------------------------------------

void some_function () {
  for (int i = 0; i < 1000; i++) {
    int *my_obj = mem_alloc(allocator, sizeof(int));
  }
}

int main (int argc, char **argv) {
  alloc_t *allocator = alloc_init();
  if (allocator == NULL) exit(-1);

------------------------------------------------------

  some_function();

------------------------------------------------------

  return 0;

}

```

