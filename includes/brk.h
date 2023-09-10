#ifndef BRK_H
# define BRK_H
// By default, kernel allocator is used
// This macro allows us to use a test double in tests
#include "heap.h"
#include "paging.h"


int brk(void *end_address);

int sbrk(u32int size);


#endif
