#include "physheap.h"

// Reserve 16
# define RESERVED_BYTES 16 * 1024

u32int mem[RESERVED_BYTES / 4];

phys_heap_t *pheap;

void init_phys_heap() {
    pheap = KALLOCATE(sizeof(phys_heap_t));
    pheap->heap = heap_place(&mem, RESERVED_BYTES, 0, 0);
    pheap->is_initialized = 1;
}

void *physical_allocate(size_t size) {
    if (pheap != NULL && pheap->is_initialized) {
        return allocate(&pheap->heap, size);
    }
    return (void *)internal_kmalloc(size);
}

void physical_free(void *addr) {
    if (pheap != NULL && pheap->is_initialized) {
        deallocate(&pheap->heap, addr);
        return;
    }
    // If the physical heap is not initialized,
    // the memory was allocated in placement_address, so we can't free it
    return;
}

size_t physical_size(void *addr) {
    if (pheap != NULL && pheap->is_initialized) {
        return memory_size(addr);
    }
    // If the physical heap was not initialized,
    // the memory was allocated in placement_address, so we can't
    // know the size
    return 0;
}

int physical_brk(void *addr) {
    // Physical heap can't be extended since it lives
    // in a pre-reserved memory space. Frames after it are likely
    // to be taken already. Just use the virtual heap :)
    return -1;
}

