#include "idt.h"
 
// A struct describing an interrupt gate.
struct idt_entry_struct
{
   u16int base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
   u16int sel;                 // Kernel segment selector.
   u8int  always0;             // This must always be zero.
   u8int  flags;               // More flags. See documentation.
   u16int base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

// A struct describing a pointer to an array of interrupt handlers.
// This is in a format suitable for giving to 'lidt'.
struct idt_ptr_struct
{
   u16int limit;
   u32int base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;
extern void idt_flush(u32int);
static void idt_set_gate(u8int,u32int,u16int,u8int);
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;

// These extern directives let us access the addresses of our ASM ISR handlers.
extern void isr0();
extern void isr1 ();
extern void isr2 ();
extern void isr3 ();
extern void isr4 ();
extern void isr5 ();
extern void isr6 ();
extern void isr7 ();
extern void isr8 ();
extern void isr9 ();
extern void isr10 ();
extern void isr11 ();
extern void isr12 ();
extern void isr13 ();
extern void isr14 ();
extern void isr15 ();
extern void isr16 ();
extern void isr17 ();
extern void isr18 ();
extern void isr19 ();
extern void isr20 ();
extern void isr21 ();
extern void isr22 ();
extern void isr23 ();
extern void isr24 ();
extern void isr25 ();
extern void isr26 ();
extern void isr27 ();
extern void isr28 ();
extern void isr29 ();
extern void isr30 ();
extern void isr31 ();


extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10 ();
extern void irq11 ();
extern void irq12 ();
extern void irq13 ();
extern void irq14 ();
extern void irq15 ();
#define ICW1_ICW4	0x01		/* Indicates that ICW4 will be present */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */
 /*I/O functions */
#define PIC1 0x20 /*IO base address primary PIC*/
#define PIC2 0xA0 /*IO secondary PIC*/
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)


#define PIC_EOI		0x20		/* End-of-interrupt command code */
 
void PIC_sendEOI(unsigned char irq)
{
	if(irq >= 8)
		outb(PIC2_COMMAND,PIC_EOI);
 
	outb(PIC1_COMMAND,PIC_EOI);
}

/*Receives value from I/O location*/

static inline void io_wait(void)
{
    outb(0x80, 0);
}
/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */
 

// A few defines to make life a little easier
#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47


typedef void (*isr_t)(registers_t);

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler)
{

  interrupt_handlers[n] = handler;
}
/*
arguments:
	offset1 - vector offset for master PIC
		vectors on the master become offset1..offset1+7
	offset2 - same for slave PIC: offset2..offset2+7
*/


uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

/*This function send value to IO location*/
void outb(uint16_t port, uint8_t val)
{
	asm volatile( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

void PIC_remap(int offset1, int offset2)
{
	// unsigned char a1, a2;
 
	// a1 = inb(PIC1_DATA);                        // save masks
	// a2 = inb(PIC2_DATA);
 
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, 0x0);   // restore saved masks.
	outb(PIC2_DATA, 0x0);
}



void init_idt()
{
   idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
   idt_ptr.base  = (u32int)&idt_entries;

   memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
   memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
   PIC_remap(0x20, 0x28);
   asm("sti");

   idt_set_gate( 0, (u32int)isr0 , 0x08, 0x8E);
   idt_set_gate( 1, (u32int)isr1 , 0x08, 0x8E);
   idt_set_gate( 2, (u32int)isr2 , 0x08, 0x8E);
   idt_set_gate( 3, (u32int)isr3 , 0x08, 0x8E);
   idt_set_gate( 4, (u32int)isr4 , 0x08, 0x8E);
   idt_set_gate( 5, (u32int)isr5 , 0x08, 0x8E);
   idt_set_gate( 6, (u32int)isr6 , 0x08, 0x8E);
   idt_set_gate( 7, (u32int)isr7 , 0x08, 0x8E);
   idt_set_gate( 8, (u32int)isr8 , 0x08, 0x8E);
   idt_set_gate( 9, (u32int)isr9 , 0x08, 0x8E);
   idt_set_gate( 10, (u32int)isr10 , 0x08, 0x8E);
   idt_set_gate( 11, (u32int)isr11 , 0x08, 0x8E);
   idt_set_gate( 12, (u32int)isr12 , 0x08, 0x8E);
   idt_set_gate( 13, (u32int)isr13 , 0x08, 0x8E);
   idt_set_gate( 14, (u32int)isr14 , 0x08, 0x8E);
   idt_set_gate( 15, (u32int)isr15 , 0x08, 0x8E);
   idt_set_gate( 16, (u32int)isr16 , 0x08, 0x8E);
   idt_set_gate( 17, (u32int)isr17 , 0x08, 0x8E);
   idt_set_gate( 18, (u32int)isr18 , 0x08, 0x8E);
   idt_set_gate( 19, (u32int)isr19 , 0x08, 0x8E);
   idt_set_gate( 20, (u32int)isr20 , 0x08, 0x8E);
   idt_set_gate( 21, (u32int)isr21 , 0x08, 0x8E);
   idt_set_gate( 22, (u32int)isr22 , 0x08, 0x8E);
   idt_set_gate( 23, (u32int)isr23 , 0x08, 0x8E);
   idt_set_gate( 24, (u32int)isr24 , 0x08, 0x8E);
   idt_set_gate( 25, (u32int)isr25 , 0x08, 0x8E);
   idt_set_gate( 26, (u32int)isr26 , 0x08, 0x8E);
   idt_set_gate( 27, (u32int)isr27 , 0x08, 0x8E);
   idt_set_gate( 28, (u32int)isr28 , 0x08, 0x8E);
   idt_set_gate( 29, (u32int)isr29 , 0x08, 0x8E);
   idt_set_gate(30, (u32int)isr30, 0x08, 0x8E);
   idt_set_gate(31, (u32int)isr31, 0x08, 0x8E);

   // PIC_remap(0x20, 0x28);   

   idt_set_gate(32, (u32int)irq0, 0x08, 0x8E);
   idt_set_gate(33, (u32int)irq1, 0x08, 0x8E);
   idt_set_gate(34, (u32int)irq2, 0x08, 0x8E);
   idt_set_gate(35, (u32int)irq3, 0x08, 0x8E);
   idt_set_gate(36, (u32int)irq4, 0x08, 0x8E);
   idt_set_gate(37, (u32int)irq5, 0x08, 0x8E);
   idt_set_gate(38, (u32int)irq6, 0x08, 0x8E);
   idt_set_gate(39, (u32int)irq7, 0x08, 0x8E);
   idt_set_gate(40, (u32int)irq8, 0x08, 0x8E);
   idt_set_gate(41, (u32int)irq9, 0x08, 0x8E);
   idt_set_gate(42, (u32int)irq10, 0x08, 0x8E);
   idt_set_gate(43, (u32int)irq11, 0x08, 0x8E);
   idt_set_gate(44, (u32int)irq12, 0x08, 0x8E);
   idt_set_gate(45, (u32int)irq13, 0x08, 0x8E);
   idt_set_gate(46, (u32int)irq14, 0x08, 0x8E);
   idt_set_gate(47, (u32int)irq15, 0x08, 0x8E);
   register_interrupt_handler(IRQ1, &keyboard_handler);
   // init_timer(50);
   idt_flush((u32int)&idt_ptr);

}

// This gets called from our ASM interrupt handler stub.
void irq_handler(registers_t regs)
{
   // terminal_writestring("recieved interrupt bruh: ");
   // ft_putnbr(regs.int_no);

   // Send an EOI (end of interrupt) signal to the PICs.
   // If this interrupt involved the slave.
   if (regs.int_no >= 40)
   {
       // Send reset signal to slave.
       outb(0xA0, 0x20);
   }
   // Send reset signal to master. (As well as slave, if necessary).
   outb(0x20, 0x20);

   if (interrupt_handlers[regs.int_no] != 0)
   {
       isr_t handler = interrupt_handlers[regs.int_no];
       handler(regs);
   }
}
static void idt_set_gate(u8int num, u32int base, u16int sel, u8int flags)
{
   idt_entries[num].base_lo = base & 0xFFFF;
   idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

   idt_entries[num].sel     = sel;
   idt_entries[num].always0 = 0;
   // We must uncomment the OR below when we get to using user-mode.
   // It sets the interrupt gate's privilege level to 3.
   idt_entries[num].flags   = flags /* | 0x60 */;
}

// This gets called from our ASM interrupt handler stub.
void isr_handler(registers_t regs)
{
    if (interrupt_handlers[regs.int_no] != 0)
    {
        isr_t handler = interrupt_handlers[regs.int_no];
        handler(regs);
    }
}

// u32int tick = 0;

// static void timer_callback(registers_t regs)
// {
//    tick++;
//    // terminal_writestring("Tick: ");
//    // terminal_writestring(tick);
//    // terminal_writestring("\n");
// }

// void init_timer(u32int frequency)
// {
//    // Firstly, register our timer callback.
//    register_interrupt_handler(IRQ0, &timer_callback);
// 	terminal_writestring("timer initz");

//    // The value we send to the PIT is the value to divide it's input clock
//    // (1193180 Hz) by, to get our required frequency. Important to note is
//    // that the divisor must be small enough to fit into 16-bits.
//    u32int divisor = 1193180 / frequency;

//    // Send the command byte.
//    outb(0x43, 0x36);

//    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
//    u8int l = (u8int)(divisor & 0xFF);
//    u8int h = (u8int)( (divisor>>8) & 0xFF );

//    // Send the frequency divisor.
//    outb(0x40, l);
//    outb(0x40, h);
// }