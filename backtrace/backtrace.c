#include "backtrace.h"

void backtrace(unsigned int maxFrames)
{
    struct stackframe *stk;
    asm ("movl %%ebp,%0" : "=r"(stk) ::);

    for(unsigned int frame = 0; stk && frame < maxFrames; ++frame)
    {
      addr2name(*(uint32_t *)(stk->eip-0x4) + stk->eip);
      stk = stk->ebp;
    }
}