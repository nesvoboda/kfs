#ifndef HEAP_H
# define HEAP_H

# include "types.h"
# include "oarray.h"
# include "allocate.h"

#define HEAP_INDEX_SIZE   0x20000
#define size_with_buffers(x) (x + sizeof(header_t) + sizeof(footer_t))
#define usable_size(x) (x - sizeof(header_t) - sizeof(footer_t))

#define KHEAP_START         0xC0000000
#define KHEAP_INITIAL_SIZE  0x100000
#define HEAP_INDEX_SIZE   0x20000
#define HEAP_MAGIC        0xDEADBEEF
#define HEAP_MIN_SIZE     0x70000

typedef struct heap_s {
    void *data;
    void *start;
    u32int size;
    int is_kernel;
    int is_readonly;
    oarray_t holes;
} _heap_t;

_heap_t *heap_create(u32int size, int is_kernel, int is_readonly);

_heap_t heap_place(void *addr, u32int size, int is_kernel, int is_readonly);

void *allocate(_heap_t *h, u32int size);

void free(void *addr);

typedef struct header_s {
    // Header is ALWAYS the user-accessible TRUE size
    // DOES NOT INCLUDE HEADER AND FOOTER SIZE
    u32int size;
    int is_hole;
} header_t;

typedef struct footer_s {
    void *to_header;
} footer_t;

size_t memory_size(void *addr);
void deallocate(_heap_t *h, void *addr);
void extend_heap(_heap_t *kheap, void *new_end_address);

#endif
