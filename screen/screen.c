#include "screen.h"

static inline u8int vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline u16int vga_entry(unsigned char uc, u8int color)
{
	return (u16int)uc | (u16int)color << 8;
}

size_t terminal_row;
size_t terminal_column;
u8int terminal_color;
u16int* terminal_buffer;

void terminal_initialize(void)
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = (u16int*)0xB8000;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}

void terminal_setcolor(u8int color)
{
	terminal_color = color;
}

void terminal_putentryat(char c, u8int color, size_t x, size_t y)
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c)
{
	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}

void terminal_write(const char* data, size_t size)
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
	terminal_write(data, strlen(data));
}

void terminal_writestring_pos(const char* data, int x, int y)
{
	int local_terminal_column = x;
	int local_terminal_row = y;

	for (size_t i = 0; i < strlen(data); i++) {
		terminal_putentryat(data[i], terminal_color, local_terminal_column, local_terminal_row);
		if (++local_terminal_column == VGA_WIDTH) {
			local_terminal_column = 0;
			if (++local_terminal_row == VGA_HEIGHT)
				local_terminal_row = 0;
		}
	}
}

void ft_putnbr_pos(int n, int x, int y)
{
	int nbr;

	if (n < 0) {
		terminal_putentryat('-', terminal_color, x++, y++);
		nbr = (n == -2147483648) ? 2147483647 : -n;
	} else
		nbr = n;
	if (nbr > 9) {
		ft_putnbr_pos(nbr / 10, x, y);
		x += numlen(nbr / 10);
		ft_putnbr_pos(nbr % 10, x++, y);
	}
	if (nbr <= 9) {
		terminal_putentryat(nbr + '0', terminal_color, x, y);
	}
}

void ft_putnbr(int n)
{
	ft_putnbr_pos(n, terminal_row, terminal_column);
	terminal_column += numlen(n);
}

int numlen(int n)
{
	int i = 0;
	while (n > 0) {
		n /= 10;
		i++;
	}
	return i;
}
