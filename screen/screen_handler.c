#include "screen_handler.h"
#include "cursor.h"

int current_index[SCREEN_MAX];

int current_start_position[SCREEN_MAX];

void init_screen() {
    for (int i = 0; i < SCREEN_MAX; i++) {
        current_index[i] = 0;
        current_start_position[i] = 0;
    }
}

int _len_to_print(int screen_no, int start_position) {
    return get_len(screen_no) - start_position;
}

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}

void _refresh_status_zone(int screen_no) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat('.',
            vga_entry_color(
                VGA_COLOR_LIGHT_BLUE,
                VGA_COLOR_LIGHT_BLUE
            ), x, 0);
    }
    uint8_t text_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_BLUE);
    terminal_setcolor(text_color);
    terminal_writestring_pos("blashOS - screen ", 30, 0);
    terminal_putentryat(screen_no + '0',text_color, 47, 0);
    for (int y = 1; y < START_LINE; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat('.',
                vga_entry_color(
                    VGA_COLOR_DARK_GREY,
                    VGA_COLOR_DARK_GREY
                ), x, y);
        }
    }
    terminal_setcolor(vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_DARK_GREY));
    terminal_writestring_pos("select color: alt + [0-9] | switch screen: alt + tab", 14, START_LINE-1);
}

void _refresh_text_zone(int screen_no) {
    text_char_t *text = get_text(screen_no);
    // invariant: never too many characters to write
    int start_writing_at = current_start_position[screen_no]; 

    int char_position = start_writing_at;

    for (int y = START_LINE; y < END_LINE; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            if (char_position < get_len(screen_no)) {
                terminal_putentryat(text[char_position].c, text[char_position].color, x, y);
            } else {
                terminal_putentryat(' ',  text[get_len(screen_no) - 1].color, x, y);
            }
            char_position++;
        }
    }
}


void _print_log_line(int index) {
    uint8_t log_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    struct log *logs = get_logs();

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
        terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_GREEN));
        terminal_writestring_pos("INFO", 11, line_row);        
    } else if (logs[index].log_level == ERROR) {
        terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_LIGHT_RED));
        terminal_writestring_pos("ERR ", 11, line_row);
    }
    terminal_putentryat(' ', log_color, 15, line_row);
    // Print message
    terminal_setcolor(log_color);
    terminal_writestring_pos(logs[index].line, 16, line_row);
}

void refresh_logs() {
    for (int y = END_LINE+1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat(' ', vga_entry_color(VGA_COLOR_BLACK, VGA_COLOR_BLACK), x, y);
        }
    }

    // draw separator
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat(' ',
            vga_entry_color(
                VGA_COLOR_DARK_GREY,
                VGA_COLOR_DARK_GREY
            ), x, END_LINE);
    }
    for (int y = 0; y < LOG_LINES; y++) {
        _print_log_line(y);
    }
}

void refresh_screen(int screen_no) {
    


    update_cursor(current_index[screen_no] - current_start_position[screen_no]);

    // update status zone
    _refresh_status_zone(screen_no);

    // update text zone
    _refresh_text_zone(screen_no);
    // update logs

    refresh_logs();

}

void screen_add_char(char c, uint8_t color, int screen_no) {
    text_char_t new_char;
    new_char.c = c;
    new_char.color = color;

    // Write character to text layer
    int res = insert_character(new_char, current_index[screen_no], screen_no);

    if (res != 0) {
        return;
    }

    current_index[screen_no] += 1;

    // If writing causes scroll
    if (_len_to_print(screen_no, current_start_position[screen_no]) > TEXT_AREA_SIZE) {
        current_start_position[screen_no] += 80;
    }

    // Else
    refresh_screen(screen_no);
}

void screen_erase(int screen_no) {
    int res = remove_character(current_index[screen_no], screen_no);
    if (res != 0) {
        return;
    }

    current_index[screen_no] -= 1;

    // If erasing causes scroll
    if (_len_to_print(screen_no, current_start_position[screen_no]) < 1) {
        // Go back one whole screen
        current_start_position[screen_no] -= TEXT_AREA_SIZE;

        //Reset to 0 if it goes below 0
        if (current_start_position[screen_no] < 0) {
            current_start_position[screen_no] = 0;
        } 
    }

    // Else
    refresh_screen(screen_no);
}

void _scroll_left(int screen_no) {
    if (current_index[screen_no] == 0) {
        return;
    }

    current_index[screen_no] -= 1;
    // TODO cursor?

    if (current_index[screen_no] <= current_start_position[screen_no]) {
        // Go back one whole screen
        current_start_position[screen_no] -= TEXT_AREA_SIZE;

        //Reset to 0 if it goes below 0
        if (current_start_position[screen_no] < 0) {
            current_start_position[screen_no] = 0;
        } 
    }
}

/// abc len 3 
void _scroll_right(int screen_no) {
    if (current_index[screen_no] >= get_len(screen_no)) {
        return;
    }

    current_index[screen_no] += 1;
    // TODO cursor?

    if (current_index[screen_no] >= current_start_position[screen_no] + TEXT_AREA_SIZE) {
        current_start_position[screen_no] += 80;
    }
}

void _scroll_up(int screen_no) {
    if (current_index[screen_no] < 80) {
        return;
    }

    current_index[screen_no] -= 80;
    // TODO cursor?

    if (current_index[screen_no] <= current_start_position[screen_no]) {
        // Go back one whole screen
        current_start_position[screen_no] -= TEXT_AREA_SIZE;

        //Reset to 0 if it goes below 0
        if (current_start_position[screen_no] < 0) {
            current_start_position[screen_no] = 0;
        } 
    }
}

void _scroll_down(int screen_no) {
    if ((current_index[screen_no]+80) >= (get_len(screen_no))) {
        current_index[screen_no] = get_len(screen_no);
    } else {
        current_index[screen_no] += 80;
    }


    // TODO cursor?

    if (current_index[screen_no] >= current_start_position[screen_no] + TEXT_AREA_SIZE) {
        current_start_position[screen_no] += 80;
    }
}

void screen_handle_scroll(enum direction dir, int screen_no) {
    if (dir == LEFT) {
        _scroll_left(screen_no);
    } else if (dir == RIGHT) {
        _scroll_right(screen_no);
    } else if (dir == TOP) {
        _scroll_up(screen_no);
    } else if (dir == BOTTOM) {
        _scroll_down(screen_no);
    }
    refresh_screen(screen_no);
}