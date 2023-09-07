#ifndef INTERNAL_ALLOCATE_H
#define INTERNAL_ALLOCATE_H

# include "types.h"

u32int get_placement_address();

u32int internal_kmalloc_a(u32int size);

// returns a physical address.
u32int internal_kmalloc_p(u32int size, u32int *phys);

// page aligned and returns a physical address.
u32int internal_kmalloc_ap(u32int size, u32int *phys);

// vanilla (normal).
u32int internal_kmalloc(u32int size);

#endif