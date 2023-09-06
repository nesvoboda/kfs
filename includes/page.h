#ifndef PAGE_H
# define PAGE_H

# include "types.h"
# include "bitset.h"
# include "utils.h"
# include "allocate.h"

typedef struct page
{
    u32int present  : 1; // Page is present in memory
    u32int rw       : 1; // Write enabled
    u32int user     : 1; // user-mode or kernel mode
    u32int accessed : 1; // Page accessed? Set by CPU
    u32int dirty    : 1; // Page written to
    u32int unused   : 7; // We can't use this
    u32int frame    : 20; // Frame adress (shifted right 12 bits) 
} page_t;

typedef struct page_table
{
    page_t pages[1024];
} page_table_t;

// Intel's tricks to lazily initialize page structure
typedef struct page_directory
{
    // Pointers to page tables
    page_table_t *tables[1024];
    // Table of physical addresses
    u32int tablesPhysical[1024];
    // Physical address of tablesPhysical
    u32int physicalAddr;
} page_directory_t;


// physical memory manager
typedef struct s_pmem_manager
{
    t_bitset frames;
    page_directory_t *directory;
    u32int total_size;
} pmem_manager_t;

pmem_manager_t init_pmem_manager(int memory_size);

page_t *_get_page(pmem_manager_t *manager, u32int virtual_address);

void map(pmem_manager_t *manager, u32int physical_address, u32int virtual_address, int is_kernel, int is_writeable);


#endif