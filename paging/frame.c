typedef struct page {
	u32int present : 1; // Page is present in memory
	u32int rw : 1; // Write enabled
	u32int user : 1; // user-mode or kernel mode
	u32int accessed : 1; // Page accessed? Set by CPU
	u32int dirty : 1; // Page written to
	u32int unused : 7; // We can't use this
	u32int frame : 20; // Frame adress (shifted right 12 bits)
} page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

// Intel's tricks to lazily initialize page structure
typedef struct page_directory {
	// Pointers to page tables
	page_table_t* tables[1024];
	// Table of physical addresses
	u32int tablesPhysical[1024];
	// Physical address of tablesPhysical
	u32int physicalAddr;
} page_directory_t;
