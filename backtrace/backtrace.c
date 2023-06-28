#include "backtrace.h"

void backtrace(unsigned int maxFrames)
{
	struct stackframe* stk;
	asm("movl %%ebp,%0"
		: "=r"(stk)::);
	char* name;

	write(1, "Backtrace: (last function first):\n", 34);

	for (unsigned int frame = 0; stk && frame < maxFrames; ++frame) {
		u32int addr = *((uint32_t*)(stk->eip - 0x4)) + stk->eip;
		name = addr2name((void * )addr);
		if (!ft_strncmp(name, "kernel_main", 11)) {
			printf("In 0x%x: %s\n", addr, name);	
		} else {
			printf("In 0x%x: %s + 0x%x\n", addr, name, (u32int) addr - stk->eip);
		}
		
		stk = stk->ebp;
	}
}