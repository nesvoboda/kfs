#include "screen_handler.h"
#include "cursor.h"

int current_index[SCREEN_MAX];

int current_start_position[SCREEN_MAX];

// The line at which the text display starts
#define START_LINE 2
#define END_LINE 15

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
    terminal_writestring_pos("blash os - screen ", 30, 0);
    terminal_putentryat(screen_no + '0',text_color, 48, 0);
    for (int y = 1; y < START_LINE; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat('.',
                vga_entry_color(
                    VGA_COLOR_DARK_GREY,
                    VGA_COLOR_DARK_GREY
                ), x, y);
        }
    }
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

void _refresh_logs(int screen_no) {
    for (int x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat(' ',
            vga_entry_color(
                VGA_COLOR_DARK_GREY,
                VGA_COLOR_DARK_GREY
            ), x, END_LINE);
    }
}

void refresh_screen(int screen_no) {
    


    update_cursor(current_index[screen_no] - current_start_position[screen_no]);

    // update status zone
    _refresh_status_zone(screen_no);

    // update text zone
    _refresh_text_zone(screen_no);
    // update logs

    _refresh_logs(screen_no);

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
    if (_len_to_print(screen_no, current_start_position[screen_no]) > 2000) {
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
        current_start_position[screen_no] -= 2000;

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
        current_start_position[screen_no] -= 2000;

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

    if (current_index[screen_no] >= current_start_position[screen_no] + 2000) {
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
        current_start_position[screen_no] -= 2000;

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

    if (current_index[screen_no] >= current_start_position[screen_no] + 2000) {
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