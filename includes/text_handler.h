#ifndef TEXT_HANDLER_H
#define TEXT_HANDLER_H

#include "config.h"
#include "screen.h"

int get_chars_until_newline(int pos);

typedef struct text_char {
	char c;
	uint8_t color;
} text_char_t;

int insert_character(text_char_t c, int index);
int remove_character(int index);
text_char_t* get_text();
int get_len();

#endif