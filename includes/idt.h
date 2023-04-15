#ifndef IDT_H
# define IDT_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>
# include "utils.h"
# include "screen.h"

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef struct registers
{
   u32int ds;                  // Data segment selector
   u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   u32int int_no, err_code;    // Interrupt number and error code (if applicable)
   u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

void init_idt();
uint8_t inb(uint16_t port);

void outb(uint16_t port, uint8_t val);
void init_timer(u32int frequency);
void keyboard_handler(registers_t regs);
void switch_to_first_terminal_buffer(void);
void switch_to_second_terminal_buffer(void);

#endif