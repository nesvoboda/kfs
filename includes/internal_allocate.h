#ifndef INTERNAL_ALLOCATE_H
#define INTERNAL_ALLOCATE_H

# include "types.h"

# ifdef TEST
#  define END NULL
# else
#  define END &end
# endif

void *get_placement_address();

void set_placement_address(u32int);

void *internal_kmalloc_a(u32int size);

// returns a physical address.
void *internal_kmalloc_p(u32int size, u32int *phys);

// page aligned and returns a physical address.
void *internal_kmalloc_ap(u32int size, u32int *phys);

// vanilla (normal).
void *internal_kmalloc(u32int size);

#endif
