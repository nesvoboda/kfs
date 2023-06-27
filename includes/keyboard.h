#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"
#include "direction.h"
#include "io.h"
#include "screen_handler.h"

#define KBD_DATA_REG 0x60 /* I/O port for keyboard data */
#define KBD_SCANCODE_MASK 0x7f
#define KBD_STATUS_MASK 0x80

enum special_char {
	TAB = 15,
	ALT = 56,
	ALT_REALEASE = -72,
	BACKSPAPCE = 14,
	ARROW = -32,
	UP_PRESSED = 72,
	BOTTOM_PRESSED = 80,
	RIGHT_PRESSED = 77,
	LEFT_PRESSED = 75
};

typedef struct keyboard_flags {
	int arrow_flag;
	int alt_flag;
} keyboard_flags_t;

void enable_keyboard();
void disable_keyboard();

#endif