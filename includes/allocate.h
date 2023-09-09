#ifndef ALLOCATE_H
# define ALLOCATE_H
// By default, kernel allocator is used
// This macro allows us to use a test double in tests
#include "internal_allocate.h"

#ifdef TEST
# include <stdlib.h>
// Real-life, host-machine malloc is used for tests
# define KALLOCATE(size) malloc(size)
  #define KALLOCATE_P(size, ptr) malloc(size)
  #define KALLOCATE_A(size) malloc(size)
#else

  #define KALLOCATE(size) internal_kmalloc(size)
  #define KALLOCATE_A(size) internal_kmalloc_a(size)
  #define KALLOCATE_P(size, ptr) internal_kmalloc_ap(size, ptr)
#endif

u32int internal_kmalloc_a(u32int size);

// returns a physical address.
u32int internal_kmalloc_p(u32int size, u32int *phys);

// page aligned and returns a physical address.
u32int internal_kmalloc_ap(u32int size, u32int *phys);

// vanilla (normal).
u32int internal_kmalloc(u32int size);


#endif
