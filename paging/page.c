#include "page.h"

pmem_manager_t init_pmem_manager(int memory_size) {
    pmem_manager_t manager;
    manager.frames = bitset_create(memory_size / 0x1000);
    manager.total_size = memory_size;

    manager.directory = KALLOCATE_A(sizeof(page_directory_t));

    // init page directory
    for (int i =0 ; i< 1024; i++) {
        manager.directory->tables[i] = NULL;
        manager.directory->tablesPhysical[i] = NULL;
    }
    // memset(&manager.directory.tables, 0, 1024);
    // memset(&manager.directory.tablesPhysical, 0, 1024);

    // init pag
    return manager;
}

// // just find any frame to map a virtual address
void _alloc_frame(pmem_manager_t *manager, u32int virtual_address, int is_kernel, int is_writeable) {
    u32int frame_idx = bitset_first_unset(manager->frames);
    map(manager, frame_idx * 0x1000, virtual_address, is_kernel, is_writeable);
};

// Creates a page for a given virtual address and a physical address
// Will currently break things if maps a physical address / virtual address that is already mapped
void map(pmem_manager_t *manager, u32int physical_address, u32int virtual_address, int is_kernel, int is_writeable) {

    page_t *target_page = _get_page(manager, virtual_address);

    target_page->present = 1;
    target_page->frame = physical_address >> 12;
    target_page->rw = is_writeable ? 1 : 0;
    target_page->user = is_kernel ? 0 : 1;
    target_page->dirty = 0;
    target_page->unused = 0;

    // Mark frame as mapped
    bitset_set(manager->frames, physical_address / 0x1000);
}

page_t *_get_page(pmem_manager_t *manager, u32int virtual_address) {
    u32int page_index = virtual_address / 0x1000;

    u32int page_table_index = page_index / 1024;

    page_table_t *page_table = manager->directory->tables[page_table_index];

    // Page table hasn't been allocated yet
    if (page_table == NULL) {
        u32int tmp = 0;
        page_table_t *new_table = (page_table_t *)KALLOCATE_P(sizeof(page_table_t), &tmp);

        // Make sure to init all pages to unused
        for (int i = 0; i < 1024; i++) {
            new_table->pages[i].present = 0;
            new_table->pages[i].accessed = 0;
            new_table->pages[i].dirty = 0;
            new_table->pages[i].frame = 0;
            new_table->pages[i].rw = 0;
            new_table->pages[i].unused = 0;
            new_table->pages[i].user = 0;
        }

        manager->directory->tables[page_table_index] = new_table;
        manager->directory->tablesPhysical[page_table_index] = tmp | 0x7;
    }

    return &manager->directory->tables[page_table_index]->pages[page_index % 1024];
}
