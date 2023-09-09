#include "heap.h"
#include "test.h"

#define HEAP_INDEX_SIZE   0x20000
#define size_with_buffers(x) (x + sizeof(header_t) + sizeof(footer_t))
#define usable_size(x) (x - sizeof(header_t) - sizeof(footer_t))

int header_predicate(void *a, void *b) {
    header_t *header_a = (header_t *) a;
    header_t *header_b = (header_t *)b;

    return header_a->size < header_b->size;
}

_heap_t *heap_create(u32int size, int is_kernel, int is_readonly) {
    _heap_t *h = KALLOCATE(sizeof(_heap_t));

    // There should be place too
    h->data = KALLOCATE(size);
    h->size = size;
    h->is_kernel = is_kernel;
    h->is_readonly = is_readonly;

    oarray_create(&h->holes, HEAP_INDEX_SIZE, &header_predicate);

    // Add one giant hole

    header_t *hole = (header_t *)h->data;
    hole->is_hole = 1;
    hole->size = usable_size(size);

    // Footer
    footer_t *footer = (footer_t *)(((void *)hole)+sizeof(header_t)+hole->size);
    footer->to_header = hole;

    oarray_insert(&h->holes, hole);
    
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
    
    if (next_head && next_head->is_hole)
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
        PANIC()
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


//// TESTS

void test_heap_create() {
    _heap_t *h = heap_create(500, 0, 0);

    // Checking hole state
    ASSERT_EQ(1, h->holes.len);

    header_t *hole = oarray_retrieve(&h->holes, 0);
    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(usable_size(500), hole->size);
    ASSERT_EQ(h->data, hole);

    // Header-footer
    footer_t *hole_footer = (void*)hole + hole->size + sizeof(header_t);
    ASSERT_EQ(hole, hole_footer->to_header);
}

void test_heap_simple() {
    _heap_t *h = heap_create(500, 0, 0);

    header_t *res = allocate(h, 10);

    // [heap_start][first block header][first block][first block footer]
    ASSERT_EQ((void*)h->data + sizeof(header_t), res);

    // holes state
    // there should be one hole for the rest of the space
    ASSERT_EQ(1, h->holes.len);
        
    header_t *hole = oarray_retrieve(&h->holes, 0);

    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(usable_size(500 - sizeof(header_t) - sizeof(footer_t) - 10), hole->size);

    // Checking block footer
    header_t *first_block_header = (void *) res - sizeof(header_t);
    footer_t *first_block_footer = (void*)h->data + sizeof(header_t) + 10;
    ASSERT_EQ(first_block_header, first_block_footer->to_header);

    // Checking hole footer
    header_t *first_hole_header = hole;
    footer_t *first_hole_footer = (void *)hole + sizeof(header_t) + hole->size;
    ASSERT_EQ(first_hole_header, first_hole_footer->to_header);

    void *res2 = allocate(h, 10);

    ASSERT_EQ(h->data + sizeof(header_t) + 
        10 + sizeof(footer_t) + sizeof(header_t), res2);
    
}

void test_heap_free() {
    _heap_t *h = heap_create(500, 0, 0);

    header_t *res = allocate(h, 10);

    void *tmp = res;

    deallocate(h, res);

    // there should be one hole for the rest of the space
    ASSERT_EQ(1, h->holes.len);

    header_t *hole = oarray_retrieve(&h->holes, 0);
    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(usable_size(500), hole->size);

    // Checking header-footer
    footer_t *footer = (void*)hole + hole->size + sizeof(header_t);
    ASSERT_EQ(hole, footer->to_header);

    // footer_t *hole_footer = hole + size_with_buffers()

    void *res2 = allocate(h, 10);

    // Memory is reused
    ASSERT_EQ(tmp, res2);  
}

void test_hole_choice() {
    _heap_t *h = heap_create(500, 0, 0);

    void *res1 = allocate(h, 10);
    void *res2 = allocate(h, 10);
    void *res3 = allocate(h, 10);

    // [block][block][block][hole]

    deallocate(h, res2);

    // [block][small hole][block][big hole]

    void *res5 = allocate(h, 10);

    // The allocator chooses the smaller hole
    ASSERT_EQ(res2, res5);
}

void test_hole_choice_3() {
    _heap_t *h = heap_create(500, 0, 0);

    allocate(h, 10);
    void *res2 = allocate(h, 40); // future big hole
    allocate(h, 10);
    void *res4 = allocate(h, 10); // future small hole
    allocate(h, 10);


    deallocate(h, res2);
    deallocate(h, res4);

    void *res6 = allocate(h, 10);

    // The allocator chooses the smaller hole
    ASSERT_EQ(res4, res6);
}

void test_merge_left() {
    _heap_t *h = heap_create(500, 0, 0);

    void *res1 = allocate(h, 10);
    void *res2 = allocate(h, 10);

    deallocate(h, res1);
    deallocate(h, res2);

    void *res3 = allocate(h, 20);

    // Two holes were merged, so the 20b
    // block can be placed there
    ASSERT_EQ(res1, res3);
}

void test_merge_both() {
    _heap_t *h = heap_create(500, 0, 0);

    void *res1 = allocate(h, 10);
    void *res2 = allocate(h, 10);
    void *res3 = allocate(h, 10);

    deallocate(h, res1);
    deallocate(h, res3);
    deallocate(h, res2);

    ASSERT_EQ(1, h->holes.len);

    header_t *hole = oarray_retrieve(&h->holes, 0);

    ASSERT_EQ(h->data, hole);
    ASSERT_EQ(usable_size(500), hole->size);
    ASSERT_EQ(1, hole->is_hole);
    
    footer_t *hole_footer = (void*)hole + sizeof(header_t) + hole->size;
    ASSERT_EQ(hole, hole_footer->to_header);

    void *res4 = allocate(h, 30);

    // Two holes were merged, so the 20b
    // block can be placed there
    ASSERT_EQ(res1, res4);
}

void test_size() {
    _heap_t *h = heap_create(500, 0, 0);

    void *res1 = allocate(h, 10);
    void *res2 = allocate(h, 10);
    void *res3 = allocate(h, 20);

    deallocate(h, res2);

    ASSERT_EQ(10, memory_size(res1));
    ASSERT_EQ(0, memory_size(res2));
    ASSERT_EQ(20, memory_size(res3));

}

void heap_tests() {
    test_heap_create();
    test_heap_simple();
    test_heap_free();
    test_hole_choice();
    test_hole_choice_3();
    test_merge_left();
    test_merge_both();
    test_size();
}


// Todo: size, brk