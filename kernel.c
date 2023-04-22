#include "screen.h"
#include "gdt.h"
#include "idt.h"
#include "cursor.h"
#include "screen_handler.h"
#include "ft_printf.h"
// #include "timer.h"                                         


const char * intro_text[] = {
"                                                                                ",
"    ____  __    ___   _____ __  __   ____  _____    __ __ ___                   ",
"   / __ )/ /   /   | / ___// / / /  / __ \\/ ___/   / // /|__ \\                  ",
"  / __  / /   / /| | \\__ \\/ /_/ /  / / / /\\__ \\   / // /___/ /                  ",
" / /_/ / /___/ ___ |___/ / __  /  / /_/ /___/ /  /__  __/ __/                   ",
"/_____/_____/_/  |_/____/_/ /_/   \\____//____/     /_/ /____/                   ",
"                                                                                ",
"                                                                                ",
};
void kernel_main(void) 
{

	
	gdt_install();
	init_idt();

	/* Initialize terminal interface */
	terminal_initialize();
	// init_timer(1);
	// init_timer(1);
	//ASCII ART
    int i;
	int n_array = 8;
    for (i = 0; i < n_array; i++) {
		terminal_writestring(intro_text[i]);
    }
	enable_cursor(0, 15);
	sleep(5);
	for (i = 0; i < 10; i++)
	{
		update_cursor(320 + i);
	}
	init_logs();
	// Needed for screen handler
	init_screen();
	printk(INFO, "System initialized");
	printk(ERROR, "Test error");
}