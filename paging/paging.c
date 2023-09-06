#include "paging.h"
#include "kheap.h"

heap_t *kheap;

// end is defined in the linker script.
extern u32int end;

// Defined in kheap.c
// Some address in the memory where we put our things
// until virtual memory works
u32int placement_address = &end;



u32int _kmalloc(u32int size, int align, u32int *phys) {

    // If address alignment is necessary
    // i.e. page directories / pages need to be aligned
    if (align == 1 && (placement_address & 0xFFFFF000)) {
        placement_address &= 0xFFFFF000;
        placement_address += 0x1000;
    }
    if (phys != NULL) {
        *phys = placement_address;
    }
    u32int save = placement_address;
    placement_address += size;
    return save;
}

// page aligned.
u32int kmalloc_a(u32int size) {
    return _kmalloc(size, 1, NULL);
} 

// returns a physical address.
u32int kmalloc_p(u32int size, u32int *phys) {
    return _kmalloc(size, 0, phys);
} 

// page aligned and returns a physical address.
u32int kmalloc_ap(u32int size, u32int *phys) {
    return _kmalloc(size, 1, phys);
}

// vanilla (normal).
u32int kmalloc(u32int size) {
    return _kmalloc(size, 0, NULL);
}

// Frames are stored as bits in the number below
u32int *frames;
u32int nframes;

// Full integers into the array
#define INDEX_FROM_BIT(a) (a / (8 * 4))
// The rest
#define OFFSET_FROM_BIT(a) (a % (8 * 4))


// Mark the frame as used in the bitset (bit array)
static void set_frame(u32int frame_addr)
{
    u32int frame = frame_addr / 0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

static void clear_frame(u32int frame_addr)
{
    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

static u32int test_frame(u32int frame_addr)
{
    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

static u32int first_frame()
{
    for (u32int i = 0; i < INDEX_FROM_BIT(nframes); i++) {
        // If there's still some space
        // Compares the entire int!
        if (frames[i] != 0xFFFFFFFF) {
            for (u32int j = 0; j < 32; j++)
            {
                // Look for free pages byte by byte
                u32int toTest = 0x1 << j;
                if (!(frames[i] & toTest)) {
                    return i*4*8+j;
                }
             }
        }
    }
}

#define PANIC(message) printk(KERNEL, "PANIC"); \
while (true) { \
    sleep(1);  \
}

void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
    if (page->frame != 0) {
        return; // Frame is already allocated
    }
    u32int idx = first_frame();
    if (idx == -1) {
        PANIC("No free frames!");
    }
    set_frame(idx * 0x1000);
    page->present = 1;
    
    if (is_writeable == 1) {
        page->rw = 1;
    } else {
        page->rw = 0;
    }
    
    if (is_kernel == 1) {
        page->user = 0;
    } else {
        page->user = 1;
    }
    page->frame = idx;
}

void free_frame(page_t *page)
{
    u32int frame = page->frame;
    if (!frame) {
        return; // Wasn't allocated
    }
    clear_frame(frame);
    page->frame = 0;
}

page_directory_t *current_directory;

pmem_manager_t current_manager;

void initialise_paging()
{
    // Reserve 16mb
    u32int mem_end_page = 0x1000000;

    // nframes = mem_end_page / 0x1000;
    // frames = (u32int *)kmalloc(INDEX_FROM_BIT(nframes));
    // memset(frames, 0, INDEX_FROM_BIT(nframes));

    // page_directory_t *kernel_directory = kmalloc_a(sizeof(page_directory_t));
    // memset(kernel_directory, 0, sizeof(page_directory_t));
    // current_directory = kernel_directory;



    // shadow
    current_manager = init_pmem_manager(mem_end_page);
    int i = 0;


    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        // map(&current_manager, i, i, 0, 1);
        _get_page(&current_manager, i);


    u32int addr = 0;
    // placement address is changed by kmalloc!
    while (addr < placement_address)
    {
        map(&current_manager, addr, addr, 0, 0);
        addr += 0x1000;
    }


    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        _alloc_frame(&current_manager, i, 0, 1);


        // _alloc_frame(&current_manager, i, 0, 1);
    register_interrupt_handler(14, page_fault);

    switch_page_directory(current_manager.directory);

    int *a = KHEAP_START;

    *a = 42;
    printk(INFO, "a: %d", *a);

    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

}

void switch_page_directory(page_directory_t *dir) {
    current_directory = dir;
    // Load physical address of the directory
    asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
    u32int cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t *get_page(u32int address, int make, page_directory_t *dir)
{
    address /= 0x1000;
    u32int table_idx = address / 1024;
    if (dir->tables[table_idx])
    {
        return &dir->tables[table_idx]->pages[address%1024];
    } else if (make) {
        u32int tmp;
        dir->tables[table_idx] = kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US
        return &dir->tables[table_idx]->pages[address % 1024];
    } else {
        return 0; // not found and not asked to create
    }
}

void page_fault(registers_t regs)
{
    // Save the failing address while we still can
    u32int failing_address;
    asm volatile("mov %%cr2, %0" : "=r"(failing_address));

    int present = !(regs.err_code & 0x1); // Was page not present
    int rw = regs.err_code & 0x2; // Was the fault a write operation
    int us = regs.err_code & 0x4; // Was processor in user-mode
    int reserved = regs.err_code & 0x8; // Were cpu-reserved bits of page entry overwritten?
    int ife = regs.err_code & 0x10; // Was this caused by an instruction fetch?

    printk(KERNEL, "PAGE FAULT, present %d, ro %d, us %d, res %d, ife %d", present, rw, us, reserved, ife);
    printk(KERNEL, "At %p", failing_address);
    PANIC("Page fault");

}