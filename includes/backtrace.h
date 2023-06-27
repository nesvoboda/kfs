#ifndef BACKTRACE_H
#define BACKTRACE_H
#include "elf.h"
#include "ft_printf.h"
#include "logs.h"
#include "types.h"

struct stackframe {
	struct stackframe* ebp;
	u32int eip;
};

void backtrace(unsigned int maxFrames);

#endif