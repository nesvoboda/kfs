# include "test.h"
# include "physheap.h"


u32int buffer[500];

void test_allocation_before_init() {
    // Mock actual placement address
    set_placement_address((u32int)&buffer);

    // Should have been allocated in placement address
    void *res = physical_allocate(10);
    ASSERT_EQ(buffer, res);

    // Free should have no effect
    physical_free(res);

    // Size is also not possible
    ASSERT_EQ(0, physical_size(res));
}

void test_init() {
    init_phys_heap();
    // Should have been allocated in the physical heap
    void *res = physical_allocate(10);
    ASSERT_NEQ(buffer, res);

    // Free should have effect
    physical_free(res);

    void *res2 = physical_allocate(10);

    ASSERT_EQ(res, res2);

    ASSERT_EQ(10, physical_size(res2));
}

void physical_heap_tests() {
    test_allocation_before_init();
    test_init();
}
