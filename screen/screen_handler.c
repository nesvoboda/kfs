#include "screen_handler.h"
#include "cursor.h"

int current_index;
int current_start_position;

void init_screen()
{
	current_index = 0;
	current_start_position = 0;
}

int _len_to_print(int start_position)
{
	text_char_t* text = get_text();
	int count = 0;
	for (int i = start_position; i < get_len(); i++) {
		if (text[i].c == '\n') {
			int pos_in_line = count % VGA_WIDTH;
			count += VGA_WIDTH - pos_in_line;
		} else {
			count += 1;
		}
	}
	return count;
}

static inline u8int vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

void _refresh_status_zone()
{
	for (int x = 0; x < VGA_WIDTH; x++) {
		terminal_putentryat('.',
			vga_entry_color(
				VGA_COLOR_LIGHT_BLUE,
				VGA_COLOR_LIGHT_BLUE),
			x, 0);
	}
	u8int text_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
	terminal_setcolor(text_color);
	terminal_writestring_pos("blashOS", 36, 0);
	for (int y = 1; y < START_LINE; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			terminal_putentryat('.',
				vga_entry_color(
					VGA_COLOR_DARK_GREY,
					VGA_COLOR_DARK_GREY),
				x, y);
		}
	}
	terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_DARK_GREY));
	terminal_writestring_pos("shell", 37, START_LINE - 1);
}

// 80

//

void _refresh_text_zone()
{
	text_char_t* text = get_text();
	// invariant: never too many characters to write
	int start_writing_at = current_start_position;

	int index = start_writing_at;

	for (int y = START_LINE; y < END_LINE; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			if (index < get_len()) {
				if (text[index].c == '\n') {
					// fill the line
					while (x < VGA_WIDTH) {
						terminal_putentryat(' ', text[index].color, x, y);
						x++;
					}
				}
				terminal_putentryat(text[index].c, text[index].color, x, y);
			} else {
				terminal_putentryat(' ', text[get_len() - 1].color, x, y);
			}
			index++;
		}
	}
}

void _print_log_line(int index)
{
	u8int log_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

	struct log* logs = get_logs();

	int line_row = END_LINE + index + 1;

	// IF the log is empty, we don't have to print it out
	if (logs[index].log_level == EMPTY) {
		return;
	}

	// Print tick
	terminal_setcolor(log_color);
	int tick_len = numlen(logs[index].tick);
	int zero_len = 10 - tick_len;
	for (int i = 0; i < zero_len; i++) {
		terminal_putentryat('0', log_color, i, line_row);
	}
	ft_putnbr_pos(logs[index].tick, zero_len, line_row);
	terminal_putentryat(' ', log_color, 10, line_row);

	// Print level
	if (logs[index].log_level == INFO) {
		terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_GREEN));
		terminal_writestring_pos("INFO", 11, line_row);
	} else if (logs[index].log_level == ERROR) {
		terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_LIGHT_RED));
		terminal_writestring_pos("ERR ", 11, line_row);
	}
	terminal_putentryat(' ', log_color, 15, line_row);
	// Print message
	terminal_setcolor(log_color);
	terminal_writestring_pos(logs[index].line, 16, line_row);
}

void refresh_logs()
{
	for (int y = END_LINE + 1; y < VGA_HEIGHT; y++) {
		for (int x = 0; x < VGA_WIDTH; x++) {
			terminal_putentryat(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), x, y);
		}
	}

	// draw separator
	for (int x = 0; x < VGA_WIDTH; x++) {
		terminal_putentryat(' ',
			vga_entry_color(
				VGA_COLOR_DARK_GREY,
				VGA_COLOR_DARK_GREY),
			x, END_LINE);
		terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_DARK_GREY));
		terminal_writestring_pos("kernel logs", 34, END_LINE);
	}
	for (int y = 0; y < LOG_LINES; y++) {
		_print_log_line(y);
	}
}

int cursor_location()
{
	text_char_t* text = get_text();
	int count = 0;
	for (int i = current_start_position; i < current_index; i++) {
		if (text[i].c == '\n') {
			int pos_in_line = count % VGA_WIDTH;
			count += VGA_WIDTH - pos_in_line;
		} else {
			count += 1;
		}
	}
	return count;
}

void refresh_screen()
{
	update_cursor(cursor_location());

	// update status zone
	_refresh_status_zone();

	// update text zone
	_refresh_text_zone();

	// update logs
	refresh_logs();
}

int screen_add_char(char c, u8int color)
{
	text_char_t new_char;
	new_char.c = c;
	new_char.color = color;

	// Write character to text layer
	int res;
	if (new_char.c == '\n') {
		res = insert_character(new_char, get_len());
	} else {
		res = insert_character(new_char, current_index);
	}

	if (res != 0) {
		return -1;
	}

	if (new_char.c == '\n') {
		current_index = get_len();
	} else {
		current_index += 1;
	}

	// If writing causes scroll
	if (_len_to_print(current_start_position) > TEXT_AREA_SIZE) {
		current_start_position += get_chars_until_newline(current_start_position);
		// get_real_chars()
	}

	// Else
	refresh_screen();
	return 0;
}

void screen_erase()
{
	int res = remove_character(current_index);
	if (res != 0) {
		return;
	}

	current_index -= 1;

	// If erasing causes scroll
	if (_len_to_print(current_start_position) < 1) {
		// Go back one whole screen
		current_start_position -= TEXT_AREA_SIZE;

		// Reset to 0 if it goes below 0
		if (current_start_position < 0) {
			current_start_position = 0;
		}
	}

	// Else
	refresh_screen();
}

int leftscroll_cutoff = 0;

void _scroll_left()
{
	if (current_index == 0 || current_index == leftscroll_cutoff) {
		return;
	}

	current_index -= 1;

	if (current_index <= current_start_position) {
		// Go back one whole screen
		current_start_position -= TEXT_AREA_SIZE;

		// Reset to 0 if it goes below 0
		if (current_start_position < 0) {
			current_start_position = 0;
		}
	}
}

void _scroll_right()
{
	if (current_index >= get_len()) {
		return;
	}

	current_index += 1;

	if (current_index >= current_start_position + TEXT_AREA_SIZE) {
		current_start_position += 80;
	}
}

void _scroll_up()
{
	if (current_index < 80) {
		return;
	}

	current_index -= 80;

	if (current_index <= current_start_position) {
		// Go back one whole screen
		current_start_position -= TEXT_AREA_SIZE;

		// Reset to 0 if it goes below 0
		if (current_start_position < 0) {
			current_start_position = 0;
		}
	}
}

void _scroll_down()
{
	if ((current_index + 80) >= (get_len())) {
		current_index = get_len();
	} else {
		current_index += 80;
	}

	if (current_index >= current_start_position + TEXT_AREA_SIZE) {
		current_start_position += 80;
	}
}

void screen_handle_scroll(enum direction dir)
{
	if (dir == LEFT) {
		_scroll_left();
	} else if (dir == RIGHT) {
		_scroll_right();
	} else if (dir == TOP) {
		// _scroll_up();
	} else if (dir == BOTTOM) {
		// _scroll_down();
	}
	refresh_screen();
}

void set_leftscroll_cutoff()
{
	leftscroll_cutoff = current_index;
}
