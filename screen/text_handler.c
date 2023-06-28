#include "text_handler.h"

#define MAX_TEXT 3000

typedef struct text_buffer {
	text_char_t buf[MAX_TEXT];
	int len;
} text_buffer_t;

text_buffer_t buffer;

void init_text_handler()
{
	buffer.len = 0;
}

void _move_everything_forward(int at_index)
{
	text_char_t* buf = buffer.buf;

	text_char_t save = buf[at_index];
	text_char_t second_save = buf[at_index];
	for (int i = at_index + 1; i < buffer.len + 1; i++) {
		second_save = buf[i];
		buf[i] = save;
		save = second_save;
	}
}

void _move_everything_back(int at_index)
{
	text_char_t* buf = buffer.buf;

	for (int i = at_index; i < buffer.len - 1; i++) {
		buf[i] = buf[i + 1];
	}
	buf[buffer.len - 1].c = '\0';
}

int insert_character(text_char_t c, int index)
{
	if (buffer.len == MAX_TEXT) {
		return -1;
	}

	_move_everything_forward(index);
	buffer.buf[index] = c;
	buffer.len += 1;
	return 0;
}

int remove_character(int index)
{
	if (buffer.len == 0) {
		return -1;
	}

	_move_everything_back(index);
	buffer.len -= 1;
	return 0;
}

text_char_t* get_text()
{
	return buffer.buf;
}

int get_len()
{
	return buffer.len;
}

#define MIN(x, y) x < y ? x : y

// Returns number of logical characters from pos to next new line
int get_chars_until_newline(int pos) {
	int res = 0;

	while ((res != VGA_WIDTH) && (res != (buffer.len-1)) && (buffer.buf[pos].c != '\n'))
	{
		res++;
		pos++;
	}
	return MIN(80, res+1);
}