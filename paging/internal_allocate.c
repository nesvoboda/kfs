#include "internal_allocate.h"

// end is defined in the linker script.
extern void *end;

typedef unsigned long uintptr_t;

// Some address in the memory where we put our things
// until virtual memory works
uintptr_t placement_address = (uintptr_t)END;

void *get_placement_address()
{
	return (void *)placement_address;
}

void set_placement_address(u32int addr) {
	placement_address = addr;
}

void *_internal_kmalloc(u32int size, int align, u32int* phys)
{

	// If address alignment is necessary
	// i.e. page directories / pages need to be aligned
	if (align == 1 && (placement_address & 0xFFFFF000)) {
		placement_address &= 0xFFFFF000;
		placement_address += 0x1000;
	}
	if (phys != NULL) {
		*phys = placement_address;
	}
	uintptr_t save = placement_address;
	placement_address += size;
	return (void *)save;
}

// page aligned.
void *internal_kmalloc_a(u32int size)
{
	return _internal_kmalloc(size, 1, NULL);
}

// returns a physical address.
void *internal_kmalloc_p(u32int size, u32int* phys)
{
	return _internal_kmalloc(size, 0, phys);
}

// page aligned and returns a physical address.
void *internal_kmalloc_ap(u32int size, u32int* phys)
{
	return _internal_kmalloc(size, 1, phys);
}

// vanilla (normal).
void *internal_kmalloc(u32int size)
{
	return _internal_kmalloc(size, 0, NULL);
}
