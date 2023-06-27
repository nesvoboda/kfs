#include "keyboard.h"

int current_screen = 0;
uint8_t current_color = 15;
keyboard_flags_t keyboard_flags = { 0, 0 };

const unsigned char kbdus[128] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
	'9', '0', '-', '=', '\b', /* Backspace */
	'\t', /* Tab */
	'q', 'w', 'e', 'r', /* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
	0, /* 29   - Control */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
	'\'', '`', 0, /* Left shift */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
	'm', ',', '.', '/', 0, /* Right shift */
	'*',
	0, /* Alt */
	' ', /* Space bar */
	0, /* Caps lock */
	0, /* 59 - F1 key ... > */
	0, 0, 0, 0, 0, 0, 0, 0,
	0, /* < ... F10 */
	0, /* 69 - Num lock*/
	0, /* Scroll Lock */
	0, /* Home key */
	0, /* Up Arrow */
	0, /* Page Up */
	'-',
	0, /* Left Arrow */
	0,
	0, /* Right Arrow */
	'+',
	0, /* 79 - End key*/
	0, /* Down Arrow */
	0, /* Page Down */
	0, /* Insert Key */
	0, /* Delete Key */
	0, 0, 0,
	0, /* F11 Key */
	0, /* F12 Key */
	0, /* All other keys are undefined */
};

int keyboard_enabled = 0;

void enable_keyboard()
{
	keyboard_enabled = 1;
}

void disable_keyboard()
{
	keyboard_enabled = 0;
}

void handle_scroll(enum direction dir)
{
	screen_handle_scroll(dir, current_screen);
}

void erase()
{
	// screen_erase(current_screen);
	io_erase();
}

void next_screen()
{
	if (++current_screen == SCREEN_MAX)
		current_screen = 0;

	refresh_screen(current_screen);
}

void switch_color(int color)
{
	current_color = color;
}

void add_char(char c)
{
	io_add_char(c, current_color);
	// screen_add_char(c, current_color);
}

void keyboard_handler(registers_t regs)
{
	char scancode;
	scancode = inb(KBD_DATA_REG);

	if (keyboard_enabled == 0) {
		return;
	}

	if ((scancode & KBD_STATUS_MASK) == 0) {
		if (keyboard_flags.arrow_flag && scancode == UP_PRESSED)
			handle_scroll(TOP);
		else if (keyboard_flags.arrow_flag && scancode == BOTTOM_PRESSED)
			handle_scroll(BOTTOM);
		else if (keyboard_flags.arrow_flag && scancode == LEFT_PRESSED)
			handle_scroll(LEFT);
		else if (keyboard_flags.arrow_flag && scancode == RIGHT_PRESSED)
			handle_scroll(RIGHT);
		else if (scancode == ALT)
			keyboard_flags.alt_flag = 1;
		else if (scancode == BACKSPAPCE)
			erase();
		else if (scancode != ARROW && scancode != LEFT_PRESSED && scancode != BOTTOM_PRESSED && scancode != UP_PRESSED && scancode != RIGHT_PRESSED) {
			char c = kbdus[scancode & KBD_SCANCODE_MASK];
			if (c >= 32 & c <= 127 || c == 10) {
				add_char(c);
			}
		}
	} else {
		if (scancode == ALT_REALEASE)
			keyboard_flags.alt_flag = 0;
		if (scancode == ARROW)
			keyboard_flags.arrow_flag = !keyboard_flags.arrow_flag;
	}
}
