#include "cursor.h"
#include "ft_printf.h"
#include "gdt.h"
#include "idt.h"
#include "heap.h"
#include "paging.h"
#include "screen.h"
#include "screen_handler.h"
#include "shell.h"

extern _heap_t kheap;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

const char* intro_text[] = {
	"                                                                  ",
	"    ____  __    ___   _____ __  __   ____  _____    __ __ ___     ",
	"   / __ )/ /   /   | / ___// / / /  / __ \\/ ___/   / // /|__ \\  ",
	"  / __  / /   / /| | \\__ \\/ /_/ /  / / / /\\__ \\   / // /___/ /",
	" / /_/ / /___/ ___ |___/ / __  /  / /_/ /___/ /  /__  __/ __/     ",
	"/_____/_____/_/  |_/____/_/ /_/   \\____//____/     /_/ /____/    ",
	"                                                                  ",
	"                                                                  ",
};

void kernel_main(void)
{

	gdt_install();
	init_idt();

	/* Initialize terminal interface */
	terminal_initialize();
	init_timer(50);
	disable_cursor();

	// ASCII ART

	for (int y = 0; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			terminal_putentryat(' ', vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE), x, y);
		}
	}

	terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE));

	int i;
	int n_array = 8;
	for (i = 0; i < n_array; i++) {
		terminal_writestring_pos(intro_text[i], 7, i + 6);
	}

	sleep(2 * 50);
	init_logs();
	// Needed for screen handler
	init_screen();
	refresh_screen(0);
	enable_cursor(0, 15);
	printk(INFO, "System initialized");

	initialise_paging();

	printk(INFO, "Paging initialized");
	int* a = allocate(&kheap, sizeof(int));
	*a = 0xFFFFFF;

	// TODO: 
	// - heap expansion
	// - physical heap (optional)
	// - demo

	printk(INFO, "%p", a);
	printk(INFO, "%d", *a);
	// printk(INFO, "%p", b);
	// printk(INFO, "%c", *b);
	// deallocate(&kheap, a);
	// printk(INFO, "%p", b);
	// a = allocate(&kheap, 100);
	// *a = 6;
	// printk(INFO, "%p", a);
	// printk(INFO, "%d", *a);
	// printk(INFO, "The size of a is %d\n", memory_size(a));
	// memory_map();
	printmem("addr 0xC0020000 60");
	shell();
}
// 0x7ffc665008f0:	0xacaffa85	0x00007fbf	0x66500920	0x00007ffc
