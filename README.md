# cgc
A mark and sweep garbage collector written in c

## Compile and run tests
```
make test
```

## Usage
```
#include "gc.h"

. . .

int main (int argc, char **argv) {
  alloc_t *allocator = alloc_init();
  if (allocator == NULL) exit(-1);

  . . .

  for (int i = 0; i < 1000; i++) {
    int *my_obj = mem_alloc(allocator, sizeof(int));
  }

  . . .

  return 0;
}

```
