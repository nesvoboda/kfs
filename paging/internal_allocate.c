#include "internal_allocate.h"

// end is defined in the linker script.
extern u32int end;

// Some address in the memory where we put our things
// until virtual memory works
u32int placement_address = END;

u32int get_placement_address()
{
	return placement_address;
}

void set_placement_address(u32int addr) {
	placement_address = addr;
}

u32int _internal_kmalloc(u32int size, int align, u32int* phys)
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
	u32int save = placement_address;
	placement_address += size;
	return save;
}

// page aligned.
u32int internal_kmalloc_a(u32int size)
{
	return _internal_kmalloc(size, 1, NULL);
}

// returns a physical address.
u32int internal_kmalloc_p(u32int size, u32int* phys)
{
	return _internal_kmalloc(size, 0, phys);
}

// page aligned and returns a physical address.
u32int internal_kmalloc_ap(u32int size, u32int* phys)
{
	return _internal_kmalloc(size, 1, phys);
}

// vanilla (normal).
u32int internal_kmalloc(u32int size)
{
	return _internal_kmalloc(size, 0, NULL);
}
