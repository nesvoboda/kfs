#include "paging.h"
#include "heap.h"

_heap_t kheap;

page_directory_t* current_directory;

pmem_manager_t current_manager;

void initialise_paging()
{
	// Reserve 16mb
	u32int total_available_memory = 0x1000000;

	// shadow
	current_manager = init_pmem_manager(total_available_memory);
	int i = 0;

	for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
		// map(&current_manager, i, i, 0, 1);
		_get_page(&current_manager, i);

	init_phys_heap();

	uintptr_t addr = 0;
	// placement address is changed by kmalloc!
	while (addr < (uintptr_t)get_placement_address()) {
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

	// int* a = (int*)KHEAP_START;

	// *a = 42;
	// printk(INFO, "a: %d", *a);

	_heap_t heap = heap_place((void *)KHEAP_START, KHEAP_INITIAL_SIZE, 0, 0);

	kheap = heap;
	// kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
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

	printk(ERROR, "PAGE FAULT, present %d, ro %d, us %d, res %d, ife %d", present, rw, us, reserved, ife);
	printk(ERROR, "At %p", failing_address);
	// Irrecoverable panic
	PANIC(0);
	return;
}

void extend_current_heap(void *new_address, void *end_address)
{
	for (uintptr_t i = (uintptr_t) new_address; i < (uintptr_t)end_address; i += 0x1000)
        _alloc_frame(&current_manager, i, 0, 1);
}

void memory_map() {
	printf("-- Memory map ---\n");
	// for (int i = 0; i < 1024; i++) {
		
	// }
	int start_index = 0;
	int in_region = 0;
	page_t this_page;
	for (int i = 0; i < 1024*1024; i++) {
		page_table_t *this_table = current_directory->tables[i / 1024];
		if (this_table != NULL) {
			this_page = this_table->pages[i % 1024];
		}
		int is_page_present = (this_table != NULL) && (this_page.present == 1);
		
		if (in_region == 1 && !is_page_present) {
			in_region = 0;
			page_t previous_page = current_directory->tables[(i-1)/1024]->pages[(i-1)%1024];
			printf("Region: from %p to %p, rw: %d u: %d a: %d d: %d\n", start_index * 0x1000, (i) * 0x1000, previous_page.rw, previous_page.user, previous_page.accessed, previous_page.dirty);
		}
		if (in_region == 0 && is_page_present) {
			in_region = 1;
			start_index = i; 
		}
	}
}




