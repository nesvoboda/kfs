/* Defines a GDT entry. We say packed, because it prevents the
*  compiler from doing things that it thinks is best: Prevent
*  compiler "optimization" by packing */
struct gdt_entry
{
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

/* Special pointer which includes the limit: The max bytes
*  taken up by the GDT, minus 1. Again, this NEEDS to be packed */
struct gdt_ptr
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/* Our GDT, with 3 entries, and finally our special GDT pointer */
struct gdt_entry gdt[7];
struct gdt_ptr gp;

/* This will be a function in start.asm. We use this to properly
*  reload the new segment registers */
extern void gdt_flush();

/* Setup a descriptor in the Global Descriptor Table */
void gdt_set_gate(int num, unsigned long base, unsigned long limit,
                           unsigned char access, unsigned char gran)
{
    /* Setup the descriptor base address */
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// GTD Access value
// Present bit
#define VALID_SEGMENT 1 << 7
// Privilege level bit
#define PRIVILEGE_KERNEL 0
#define PRIVILEGE_RING_1 1 << 5
#define PRIVILEGE_RING_2 2 << 5
#define PRIVILEGE_USER 3 << 5
// Type bit
#define TYPE_SYSTEM 0
#define TYPE_CODE_OR_DATA 1 << 4
// Executable bit
#define EXECUTABLE_YES 1 << 3
#define EXECUTABLE_NO 0
// Direction / conforming bit
#define DIRECTION_UP 0
#define DIRECTION_DOWN 1 << 2
// For code, this bit means confoming
// can only be executed from specified ring
#define RING_STRICT 0
// can be executed from lower rings
#define RING_ALLOW_LOWER 1 << 2

// Readable/writable bit
#define RDWR_ALLOW 1 << 1
#define RDWR_DENY 0


// Accessed bit
// set to 0, set by CPU
#include "types.h"
/* Should be called by main. This will setup the special GDT
*  pointer, set up the first 3 entries in our GDT, and then
*  finally call gdt_flush() in our assembler file in order
*  to tell the processor where the new GDT is and update the
*  new segment registers */


void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*our_dst;
	unsigned char	*our_src;

	i = 0;
	our_dst = dst;
	our_src = (unsigned char*)src;
	if (dst == NULL && src == NULL)
		return (NULL);
	while (i < n)
	{
		our_dst[i] = our_src[i];
		i++;
	}
	return (dst);
}


void gdt_install()
{
    /* Setup the GDT pointer and limit */
    gp.limit = (sizeof(struct gdt_entry) * 7) - 1;
    gp.base = 0x800;

    /* Our NULL descriptor */
    gdt_set_gate(0, 0, 0, 0, 0);

    /* The second entry is our Code Segment. The base address
    *  is 0, the limit is 4GBytes, it uses 4KByte granularity,
    *  uses 32-bit opcodes, and is a Code Segment descriptor.
    *  Please check the table above in the tutorial in order
    *  to see exactly what each value means */
    u8int kernel_code_segment_access = VALID_SEGMENT | PRIVILEGE_KERNEL | TYPE_CODE_OR_DATA | EXECUTABLE_YES | RING_STRICT | RDWR_ALLOW;
    gdt_set_gate(1, 0, 0xFFFFFFFF, kernel_code_segment_access, 0xCF);
    u8int kernel_data_segment_access = VALID_SEGMENT | PRIVILEGE_KERNEL | TYPE_CODE_OR_DATA | EXECUTABLE_NO | DIRECTION_UP | RDWR_ALLOW;

    /* The third entry is our Data Segment. It's EXACTLY the
    *  same as our code segment, but the descriptor type in
    *  this entry's access byte says it's a Data Segment */
    gdt_set_gate(2, 0, 0xFFFFFFFF, kernel_data_segment_access, 0xCF);

    u8int kernel_stack_segment_access = VALID_SEGMENT | PRIVILEGE_KERNEL | TYPE_CODE_OR_DATA | EXECUTABLE_NO | DIRECTION_DOWN | RDWR_ALLOW;
   
    gdt_set_gate(3, 0, 0x0, kernel_stack_segment_access, 0xCF);

    u8int user_code_segment_access = VALID_SEGMENT | PRIVILEGE_USER | TYPE_CODE_OR_DATA | EXECUTABLE_YES | RING_ALLOW_LOWER | RDWR_ALLOW;

    gdt_set_gate(4, 0, 0xFFFFFFFF, user_code_segment_access, 0xCF);


    u8int user_data_segment_access = VALID_SEGMENT | PRIVILEGE_USER | TYPE_CODE_OR_DATA | EXECUTABLE_NO | DIRECTION_UP | RDWR_ALLOW;
    gdt_set_gate(5, 0, 0xFFFFFFFF, user_data_segment_access, 0xCF);
    u8int user_stack_segment_access = VALID_SEGMENT | PRIVILEGE_USER | TYPE_CODE_OR_DATA | EXECUTABLE_NO | DIRECTION_DOWN | RDWR_ALLOW;
    gdt_set_gate(6, 0, 0x0, user_stack_segment_access, 0xCF);
    
    ft_memcpy((char *)0x800, (char *) gdt, gp.limit);
    /* Flush out the old GDT and install the new changes! */
    gdt_flush();
}
