#include "screen_handler.h"

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

void refresh_screen(int screen_no) {
    text_char_t *text = get_text(screen_no);

    // invariant: never too many characters to write
    int start_writing_at = current_start_position[screen_no]; 

    int char_position = start_writing_at;

    for (int y = 0; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            if (char_position < get_len(screen_no)) {
                terminal_putentryat(text[char_position].c, text[char_position].color, x, y);
            } else {
                terminal_putentryat(' ', 0, x, y);
            }
            char_position++;
        }
    }
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

/// abc
void _scroll_right(int screen_no) {
    if (current_index[screen_no] >= (get_len(screen_no))) {
        return;
    }

    current_index[screen_no] += 1;
    // TODO cursor?

    if (current_index[screen_no] >= current_start_position[screen_no] + 2000) {
        current_start_position[screen_no] += 80;
    }
}

void _scroll_up(int screen_no) {
    if (current_index[screen_no] <= 80) {
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