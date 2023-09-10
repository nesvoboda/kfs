#ifndef IDT_H
#define IDT_H

#include "screen.h"
#include "utils.h"

// Enables registration of callbacks for interrupts or IRQs.
// For IRQs, to ease confusion, use the #defines above as the
// first parameter.
typedef struct registers {
	u32int ds; // Data segment selector
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
	u32int int_no, err_code; // Interrupt number and error code (if applicable)
	u32int eip, cs, eflags, useresp, ss; // Pushed by the processor automatically.
} registers_t;

typedef void (*isr_t)(registers_t);
void init_idt();
u8int inb(u16int port);
void register_interrupt_handler(u8int n, isr_t handler);
void outb(u16int port, u8int val);
void outw(u16int port, u16int val);
void init_timer(u32int frequency);
void keyboard_handler(registers_t regs);

#endif
