#include "heap.h"
#include "test.h"

_heap_t *heap_create(u32int size, int is_kernel, int is_readonly) {
    _heap_t *h = KALLOCATE(sizeof(_heap_t));

    // There should be place too
    h->data = KALLOCATE(size);
    h->size = size;
    h->is_kernel = is_kernel;
    h->is_readonly = is_readonly;
    return h;
}


void *allocate(_heap_t *h, u32int size) {
    return NULL;
}


void heap_tests() {
    _heap_t *h = heap_create(500, 0, 0);

    void *res = allocate(h, 10);

    // [heap_start][first block header][first block][first block footer]
    ASSERT_EQ(h->data + sizeof(header_t), res);

    void *res2 = allocate(h, 10);

    ASSERT_EQ(h->data + sizeof(header_t) + 
        10 + sizeof(footer_t) + sizeof(header_t), res2);
    
}
