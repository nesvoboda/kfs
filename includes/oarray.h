#ifndef OARRAY_H
# define OARRAY_H

# include "types.h"
# include "allocate.h"
# include "panic.h"

typedef void* type_t;

typedef int(predicate) (type_t *, type_t *);

typedef struct ordered_array {
    type_t *data;
    u32int size;
    predicate *p;
    u32int len;

} oarray_t;

int lessthan(type_t a, type_t b);

#endif
