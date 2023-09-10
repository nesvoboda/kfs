#ifndef PAGING_H
# define PAGING_H

# include "page.h"
# include "types.h"
# include "idt.h" // for registers_t
# include "panic.h"


void initialise_paging();

void switch_page_directory(page_directory_t *new);
// Get a given page entry for a given address
// 1 = create if not exists
page_t *get_page(u32int address, int make, page_directory_t *dir);

void page_fault(registers_t regs);


#endif
