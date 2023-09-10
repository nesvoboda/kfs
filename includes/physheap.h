#ifndef PHYSHEAP_H
# define PHYSHEAP_H

# include "types.h"
# include "heap.h"
# include "allocate.h"

typedef struct phys_heap_s {
    _heap_t heap;
    int is_initialized;
} phys_heap_t;

void init_phys_heap();
void *physical_allocate(size_t size);
void physical_free(void *addr);
size_t physical_size(void *addr);

#endif
