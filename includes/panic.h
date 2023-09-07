#ifndef PANIC_H
# define PANIC_H

# include "ft_printf.h"

#define PAGE_FAULT(x) printk(KERNEL, "%s", x);\
  while (1) { \
  } \

#define PANIC() printk(KERNEL, "PANIC at line %d in file %s", __LINE__, __FILE__);\
  while (1) { \
  } \

#define ASSERT(x) if (!(x)) {PANIC()}


#endif