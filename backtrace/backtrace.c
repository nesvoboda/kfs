#include "backtrace.h"

void backtrace(unsigned int maxFrames)
{
	struct stackframe* stk;
	asm("movl %%ebp,%0"
		: "=r"(stk)::);
	char* name;

	for (unsigned int frame = 0; stk && frame < maxFrames; ++frame) {
		name = addr2name(*(uint32_t*)(stk->eip - 0x4) + stk->eip);
		write(1, name, ft_strlen(name));
		stk = stk->ebp;
	}
}