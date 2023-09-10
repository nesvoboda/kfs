#ifndef PANIC_H
# define PANIC_H


#ifndef TEST
# include "ft_printf.h"
  void kpanic(int recoverable);
  #define PANIC(recoverable) printk(ERROR, "PANIC at line %d in file %s", __LINE__, __FILE__);\
    kpanic(recoverable);
  #define ASSERT(x) if (!(x)) {PANIC()}

# else
# include <stdio.h>
  #define PAGE_FAULT(x) printf("%s", x);\
    while (1) { \
    } \

  #define PANIC() printf("PANIC at line %d in file %s", __LINE__, __FILE__);\
    while (1) { \
    } \

  #define ASSERT(x) if (!(x)) {PANIC()}
# endif

#endif
