#include "page.h"
#include "test.h"

void test_init_pmem_manager() {
    pmem_manager_t manager = init_pmem_manager(33);

    ASSERT_EQ(1, manager.frames.size);

    // All frames unmapped
    ASSERT_EQ(0, bitset_first_unset(manager.frames));

}

void test_map() {
    pmem_manager_t manager = init_pmem_manager(33);

    map(&manager, 0x1000*3, 0XA000, 1, 1);

    ASSERT_EQ(1, manager.directory.tables[0]->pages[0xA].present);
    ASSERT_EQ((0x1000 * 3) >> 12, manager.directory.tables[0]->pages[0xA].frame);

}

void page_tests() {
    test_init_pmem_manager();
    test_map();
}