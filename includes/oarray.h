#ifndef OARRAY_H
# define OARRAY_H

# include "types.h"
# include "allocate.h"
# include "panic.h"

typedef void* type_t;

typedef int(predicate) (type_t, type_t);

typedef struct ordered_array {
    type_t *data;
    u32int size;
    predicate *p;
    u32int len;

} oarray_t;

int lessthan(type_t a, type_t b);

void oarray_create(oarray_t* array, u32int max_size, predicate* pr);
void oarray_insert(oarray_t* array, type_t item);
void oarray_delete(oarray_t* array, size_t index);
type_t oarray_retrieve(oarray_t* array, size_t index);
oarray_t oarray_place(void *addr, u32int max_size, predicate* pr);

#endif
