#include "screen.h"
#include "gdt.h"
#include "idt.h"
 
#include "screen_handler.h"

void kernel_main(void) 
{

	
	gdt_install();
	init_idt();

	/* Initialize terminal interface */
	terminal_initialize();
	init_timer(1);

	terminal_writestring("42");

	// Test interrupt
	asm volatile ("int $0x3");

	asm volatile ("int $0x21");


	// Needed for screen handler
	init_screen();
}