#ifndef ALLOCATE_H
# define ALLOCATE_H
// By default, kernel allocator is used
// This macro allows us to use a test double in tests

#ifdef TEST
# include <stdlib.h>
// Real-life, host-machine malloc is used for tests
# define KALLOCATE(size) malloc(size)
  #define KALLOCATE_P(size, ptr) malloc(size)
#else
  #define KALLOCATE(size) kmalloc(size)
  #define KALLOCATE_A(size) kmalloc_a(size)
  #define KALLOCATE_P(size, ptr) kmalloc_ap(size, ptr)
#endif

#endif