#include "cursor.h"

void enable_cursor(u8int cursor_start, u8int cursor_end)
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void disable_cursor()
{
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void update_cursor(u16int pos)
{
	// add status zone
	pos += VGA_WIDTH * 2;
	// u16int pos = y * VGA_WIDTH + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (u8int)(pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (u8int)((pos >> 8) & 0xFF));
}
