#ifndef IDT_H
# define IDT_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "utils.h"

void init_idt();
void init_timer(u32int frequency);


#endif