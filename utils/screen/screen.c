#include "screen.h"
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}


 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint8_t terminal_color1;
uint8_t terminal_color2;

uint16_t* terminal_buffer;
uint16_t* first_terminal_buffer;
uint16_t* second_terminal_buffer;
// typedef struct screen
// {
// 	size_t terminal_rows;
// 	size_t terminal_columns;
// 	uint8_t terminal_colors;
// 	uint16_t* terminal_buffers;
// 	struct screen *next;
// 	struct screen *prev;           // The upper 16 bits of the address to jump to.
// } screen_t;

void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
	terminal_color1 = vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK);
	terminal_color2 = vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);

	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color1);
			first_terminal_buffer[index] = vga_entry(' ', terminal_color1);
			second_terminal_buffer[index] = vga_entry(' ', terminal_color2);

		}
	}

}
 
void switch_to_first_terminal_buffer(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			second_terminal_buffer[index] = terminal_buffer[index];
			terminal_buffer[index] = first_terminal_buffer[index];

		}
	}
 	terminal_color = terminal_color1;

}

void switch_to_second_terminal_buffer(void)
{
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			first_terminal_buffer[index] = terminal_buffer[index];
			terminal_buffer[index] = second_terminal_buffer[index];
		}
	}
	terminal_color = terminal_color2;
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
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

void terminal_writestring_pos(const char *data, int x, int y)
{
	int local_terminal_column = x;
	int local_terminal_row = y;
	
	for (size_t i = 0; i < strlen(data); i++)
	{
		terminal_putentryat(data[i], terminal_color, local_terminal_column, local_terminal_row);
		if (++local_terminal_column == VGA_WIDTH) {
			local_terminal_column = 0;
			if (++local_terminal_row == VGA_HEIGHT)
				local_terminal_row = 0;
		}
	}
}

void	ft_putnbr_pos(int n, int x, int y)
{
	int nbr;

	if (n < 0)
	{
		terminal_putentryat('-', terminal_color, x++, y++);
		nbr = (n == -2147483648) ? 2147483647 : -n;
	}
	else
		nbr = n;
	if (nbr > 9)
	{
		ft_putnbr_pos(nbr / 10, x, y);
		x += numlen(nbr / 10);
		ft_putnbr_pos(nbr % 10, x++, y);
	}
	if (nbr <= 9)
	{
		terminal_putentryat(nbr + '0', terminal_color, x, y);
	}
}

void	ft_putnbr(int n)
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

void	ft_putnbr_classic(int n)
{
	int nbr;

	if (n < 0)
	{
		terminal_putchar('-');
		nbr = (n == -2147483648) ? 2147483647 : -n;
	}
	else
		nbr = n;
	if (nbr > 9)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
	if (nbr <= 9)
	{
		terminal_putchar(nbr + '0');
	}
}