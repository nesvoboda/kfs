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
		name = addr2name(addr);
		write(1, name, ft_strlen(name));
		write(1, "\n", 1);
		stk = stk->ebp;
	}
}