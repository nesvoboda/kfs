#include "paging.h"
#include "kheap.h"

heap_t* kheap;

// Frames are stored as bits in the number below
u32int* frames;
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
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}

static u32int test_frame(u32int frame_addr)
{
	u32int frame = frame_addr / 0x1000;
	u32int idx = INDEX_FROM_BIT(frame);
	u32int off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}

void free_frame(page_t* page)
{
	u32int frame = page->frame;
	if (!frame) {
		return; // Wasn't allocated
	}
	clear_frame(frame);
	page->frame = 0;
}

page_directory_t* current_directory;

pmem_manager_t current_manager;

void initialise_paging()
{
	// Reserve 16mb
	u32int total_available_memory = 0x1000000;

	// nframes = mem_end_page / 0x1000;
	// frames = (u32int *)kmalloc(INDEX_FROM_BIT(nframes));
	// _memset(frames, 0, INDEX_FROM_BIT(nframes));

	// page_directory_t *kernel_directory = internal_kmalloc_a(sizeof(page_directory_t));
	// _memset(kernel_directory, 0, sizeof(page_directory_t));
	// current_directory = kernel_directory;

	// shadow
	current_manager = init_pmem_manager(total_available_memory);
	int i = 0;

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
		// map(&current_manager, i, i, 0, 1);
		_get_page(&current_manager, i);

	u32int addr = 0;
	// placement address is changed by kmalloc!
	while (addr < get_placement_address()) {
		map(&current_manager, addr, addr, 0, 0);
		addr += 0x1000;
	}
	// One more page for future
	map(&current_manager, addr, addr, 0, 0);

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
		_alloc_frame(&current_manager, i, 0, 1);

	// _alloc_frame(&current_manager, i, 0, 1);
	register_interrupt_handler(14, page_fault);

	switch_page_directory(current_manager.directory);

	int* a = (int*)KHEAP_START;

	*a = 42;
	printk(INFO, "a: %d", *a);

	kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

void switch_page_directory(page_directory_t* dir)
{
	current_directory = dir;
	// Load physical address of the directory
	asm volatile("mov %0, %%cr3" ::"r"(&dir->tablesPhysical));
	u32int cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" ::"r"(cr0));
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
	PAGE_FAULT("Page fault");
}
