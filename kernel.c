#include "cursor.h"
#include "ft_printf.h"
#include "gdt.h"
#include "idt.h"
#include "heap.h"
#include "paging.h"
#include "screen.h"
#include "screen_handler.h"
#include "shell.h"
#include "physheap.h"
#include "panic.h"
#include "brk.h"

extern _heap_t kheap;

static inline u8int vga_entry_color(enum vga_color fg, enum vga_color bg)
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

	int* a = allocate(&kheap, sizeof(int));
	*a = 0xFFFFFF;

	int *b = physical_allocate(sizeof(int));
	*b = 42;

	printk(INFO, "Initial heap size: 0x%x", kheap.size);
	// add 10 pages to the heap
	brk((void *)(KHEAP_START + KHEAP_INITIAL_SIZE + 0x10000));
	sbrk(0x10000);
	printk(INFO, "New heap size: 0x%x", kheap.size);

	printk(INFO, "Allocated a at %p, val: %d, size: %d bytes", a, *a, memory_size(a));
	
	printk(INFO, "Physical b at %p, val: %d, size: %d bytes", b, *b, physical_size(b));

	PANIC(1);
	shell();
}
