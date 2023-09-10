#include "heap.h"
#include "paging.h"

extern _heap_t kheap;
extern pmem_manager_t current_manager;


int brk(void *end_address)
{
    if ((uintptr_t)end_address < (uintptr_t)kheap.start)
        return -1;     
    if ((uintptr_t)end_address < (uintptr_t)(kheap.start + kheap.size))
        return -1;     
    header_t *new_hole_address = (void *)kheap.start + kheap.size;

    for (uintptr_t i = (uintptr_t)new_hole_address; i < (uintptr_t)end_address; i += 0x1000)
        _alloc_frame(&current_manager, i, 0, 1);
    extend_heap(&kheap, end_address);
    return 0;
}


int sbrk(u32int size)
{
    void *current_end = kheap.start + kheap.size;
    return brk(current_end + size);
}
