#include "heap.h"


int header_predicate(void *a, void *b) {
    header_t *header_a = (header_t *) a;
    header_t *header_b = (header_t *)b;

    return header_a->size < header_b->size;
}

// todo merge with place
_heap_t *heap_create(u32int size, int is_kernel, int is_readonly) {
    _heap_t *h = KALLOCATE(sizeof(_heap_t));

    int index_size = size / 8; 
    h->start = h;
    h->size = size;
    h->is_kernel = is_kernel;
    h->is_readonly = is_readonly;

    // oarray_create(&h->holes, HEAP_INDEX_SIZE, &header_predicate);
    void *addr = KALLOCATE(size);
    
    oarray_t holes = oarray_place(addr, index_size, &header_predicate);

    h->data = addr + (index_size);
    h->holes = holes;


    // Add one giant hole

    header_t *hole = (header_t *)h->data;
    hole->is_hole = 1;
    hole->size = usable_size(size - index_size);

    // Footer
    footer_t *footer = (footer_t *)(((void *)hole)+sizeof(header_t)+hole->size);
    footer->to_header = hole;

    oarray_insert(&h->holes, hole);
    
    return h;
}


_heap_t heap_place(void *addr, u32int size, int is_kernel, int is_readonly) {
    _heap_t h;

    int index_size = size / 8;
    h.size = size;
    h.start = addr;
    h.is_kernel = is_kernel;
    h.is_readonly = is_readonly;
    oarray_t holes = oarray_place(addr, index_size, &header_predicate);

    h.data = addr + index_size;
    h.holes = holes;


    // Add one giant hole

    header_t *hole = (header_t *)h.data;
    hole->is_hole = 1;
    hole->size = usable_size(size - index_size);

    // Footer
    footer_t *footer = (footer_t *)(((void *)hole)+sizeof(header_t)+hole->size);
    footer->to_header = hole;

    oarray_insert(&h.holes, hole);
    
    return h;
}

header_t *find_smallest_hole(_heap_t *h, u32int size) {
    for (int i = 0; i < h->holes.len; i++) {
        header_t *hole = (header_t *)oarray_retrieve(&h->holes, i);
        if (hole->size >= size) {
            return hole;
        }
    }
    return NULL;
}

void remove_hole(_heap_t *h, header_t *hole) {
    for (int i = 0; i < h->holes.len; i++) {
        header_t *cur = (header_t *)oarray_retrieve(&h->holes, i);
        if (cur == hole) {
            oarray_delete(&h->holes, i);
            return;
        }
    }
}

// Glues the hole with the next hole
void merge_right(_heap_t *h, header_t *current_hole) {
    header_t *next_hole = (void *)current_hole + size_with_buffers(current_hole->size);

    current_hole->size += next_hole->size + sizeof(header_t) + sizeof(footer_t);

    footer_t *next_footer = (footer_t *)((void *)next_hole + next_hole->size + sizeof(header_t));
    
    // Next footer will now point to the current header
    next_footer->to_header = current_hole;
    
    remove_hole(h, next_hole);
}

int merge_left(_heap_t *h, header_t *freed_block) {
    footer_t *prev_footer = ((footer_t *)((void *)freed_block -sizeof(footer_t)));
    header_t *prev_head = prev_footer->to_header;

    //[header][hole][footer][header][hole][footer]
    if (prev_head && prev_head->is_hole)
    {
        prev_head->size += size_with_buffers(freed_block->size);

        // Current footer will now point to the previous header
        // For some reason, current footer points to previous head
        footer_t *current_footer = (footer_t *)((void*)freed_block + freed_block->size+sizeof(header_t));
        current_footer->to_header = prev_head;
        
        // Current header does not represent a hole anymore
        // This is probably not necessary, the initial block wasn't a hole
        remove_hole(h, freed_block);
        
        // Merge performed
        return 1;
    }
    // No merge was necessary
    return 0;
}

void deallocate(_heap_t *h, void *addr)
{
    header_t *head = addr - sizeof(header_t);
    header_t *next_head = (void *)head + size_with_buffers(head->size);
    footer_t *prev_footer = ((footer_t *)((void *)head - sizeof(footer_t)));
    
    head->is_hole = 1;
    
    if ((void *)next_head < h->start + h->size && next_head && next_head->is_hole)
    {
        merge_right(h, head);
    }
    
    int merged_left = 0;
    if ((void *)prev_footer > h->data)
    {
        merged_left = merge_left(h, head);
    }
    
    if (!merged_left)
    {
        oarray_insert(&h->holes, head);
    }
}


void *allocate(_heap_t *h, u32int size) {
    header_t *hole = find_smallest_hole(h, size);

    if (hole == NULL) {
        // we don't have any more space
        PANIC(0)
    }

    hole->is_hole = 0;

    // Add footer 
    footer_t *f = (footer_t *)((void *)hole + size+sizeof(header_t));
    f->to_header = hole;

    if (hole->size > size) {
        header_t *new_hole = (void *) hole + size_with_buffers(size);
    
        new_hole->size = hole->size - size_with_buffers(size);
        new_hole->is_hole = 1;
        footer_t *new_f = (footer_t *)((void *)new_hole + new_hole->size + sizeof(header_t));
        new_f->to_header = new_hole;
        oarray_insert(&h->holes, new_hole);
    }
    hole->size = size;    remove_hole(h, hole);

    return (void*)(hole) + sizeof(header_t);
}

size_t memory_size(void *addr) {
    header_t *header = addr - sizeof(header_t);
    if (header->is_hole == 1) {
        return 0;
    }
    return header->size;
}

void extend_heap(_heap_t *kheap, void *new_end_address)
{
    header_t *new_hole_address = (header_t *)(kheap->start + kheap->size);

    new_hole_address->size =  usable_size(new_end_address - (void *)new_hole_address);
    footer_t *new_footer = (footer_t *)((void *)new_hole_address + new_hole_address->size + sizeof(header_t));
    new_footer->to_header = new_hole_address;
    kheap->size =  new_end_address - kheap->start;
    deallocate(kheap, (void *)((void *)new_hole_address + sizeof(header_t)));
}
