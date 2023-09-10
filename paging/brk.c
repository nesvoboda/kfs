#include "heap.h"
#include "paging.h"

extern _heap_t kheap;
extern pmem_manager_t current_manager;

int brk(void *end_address)
{
    if ((uintptr_t)end_address < KHEAP_START)
        return -1;     
    if ((uintptr_t)end_address < KHEAP_START + kheap.size)
        return -1;     
    header_t *new_hole_address = (void *)KHEAP_START + kheap.size;

    for (uintptr_t i = (uintptr_t)new_hole_address; i < (uintptr_t)end_address; i += 0x1000)
        _alloc_frame(&current_manager, i, 0, 1);

    new_hole_address->size =  usable_size(end_address - (kheap.data + kheap.size));
    footer_t *new_footer = (void *)new_hole_address + new_hole_address->size + sizeof(header_t);
    new_footer->to_header = new_hole_address;
    kheap.size =  end_address - kheap.data;
    deallocate(&kheap, (void *)new_hole_address + sizeof(header_t));
    return 0;
}


int sbrk(u32int size)
{
    void *current_end = kheap.data + kheap.size;
    return brk(current_end + size);
}
