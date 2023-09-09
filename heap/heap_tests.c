#include "heap.h"
#include "test.h"

//// TESTS

void test_heap_create() {
    _heap_t *h = heap_create(5000, 0, 0);

    // Checking hole state
    ASSERT_EQ(1, h->holes.len);

    header_t *hole = oarray_retrieve(&h->holes, 0);
    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(usable_size(5000 - (5000 / 8)), hole->size);
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
    ASSERT_EQ(usable_size((500 - 500 / 8) - sizeof(header_t) - sizeof(footer_t) - 10), hole->size);

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
    ASSERT_EQ(usable_size(500 - 500 / 8), hole->size);

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
    ASSERT_EQ(usable_size(500 - 500 / 8), hole->size);
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

void test_heap_place() {
    char dest[500];

    _heap_t h = heap_place(&dest, 500, 0, 0);

    header_t *res = allocate(&h, 10);

    // [heap_start][first block header][first block][first block footer]
    ASSERT_EQ((void*)h.data + sizeof(header_t), res);

    // holes state
    // there should be one hole for the rest of the space
    ASSERT_EQ(1, h.holes.len);
        
    header_t *hole = oarray_retrieve(&h.holes, 0);

    ASSERT_EQ(1, hole->is_hole);
    ASSERT_EQ(usable_size((500 - 500/ 8) - sizeof(header_t) - sizeof(footer_t) - 10), hole->size);

    // Checking block footer
    header_t *first_block_header = (void *) res - sizeof(header_t);
    footer_t *first_block_footer = (void*)h.data + sizeof(header_t) + 10;
    ASSERT_EQ(first_block_header, first_block_footer->to_header);

    // Checking hole footer
    header_t *first_hole_header = hole;
    footer_t *first_hole_footer = (void *)hole + sizeof(header_t) + hole->size;
    ASSERT_EQ(first_hole_header, first_hole_footer->to_header);
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
    test_heap_place();
}


// Todo: size, brk
