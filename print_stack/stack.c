#include "types.h"
#include "ft_printf.h"
#include "logs.h"

struct stackframe {
  struct stackframe* ebp;
  u32int eip;
};

void TraceStackTrace(unsigned int MaxFrames)
{
    char *kek;


    struct stackframe *stk;
    asm ("movl %%ebp,%0" : "=r"(stk) ::);
    printk(INFO, "Stack trace:");
    for(unsigned int frame = 0; stk && frame < MaxFrames; ++frame)
    {
        // Unwind to previous stack frame
        printk(INFO, "offset: %x", *(uint32_t *)(stk->eip-0x4));
        printk(INFO, "result: %x", *(uint32_t *)(stk->eip-0x4) + stk->eip);
        stk = stk->ebp;
    }
}