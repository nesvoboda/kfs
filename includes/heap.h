#ifndef HEAP_H
# define HEAP_H

# include "types.h"
# include "oarray.h"
# include "allocate.h"


typedef struct heap_s {
    void *data;
    u32int size;
    int is_kernel;
    int is_readonly;
    oarray_t holes;
} _heap_t;


_heap_t *heap_create(u32int size, int is_kernel, int is_readonly);

void *allocate(_heap_t *h, u32int size);

void free(void *addr);

typedef struct header_s {
    // Header is ALWAYS the user-accessible TRUE size
    // DOES NOT INCLUDE HEADER AND FOOTER SIZE
    u32int size;
    int is_hole;
} header_t;

typedef struct footer_s {
    void *to_header;
} footer_t;

#endif
