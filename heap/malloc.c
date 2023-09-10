#include "_malloc.h"

extern _heap_t kheap;

void *malloc(size_t size) {
    if (kheap.size < size) {
        return NULL;
    }
    return allocate(&kheap, size);
}
