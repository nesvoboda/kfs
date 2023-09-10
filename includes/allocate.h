#ifndef ALLOCATE_H
# define ALLOCATE_H

#ifdef TEST
  #include <stdlib.h>
// Real-life, host-machine malloc is used for tests
# define KALLOCATE(size) malloc(size)
  #define KALLOCATE_P(size, ptr) malloc(size)
  #define KALLOCATE_A(size) malloc(size)
#else
  #include "internal_allocate.h"
  #define KALLOCATE(size) internal_kmalloc(size)
  #define KALLOCATE_A(size) internal_kmalloc_a(size)
  #define KALLOCATE_P(size, ptr) internal_kmalloc_ap(size, ptr)
#endif


#endif
