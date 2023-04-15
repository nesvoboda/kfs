#include "screen_handler.h"

# define MAX_SCREENS 3

int current_index[MAX_SCREENS];

int current_start_position[MAX_SCREENS];

void init_screen() {
    for (int i = 0; i < MAX_SCREENS; i++) {
        current_index[i] = 0;
        current_start_position[i] = 0;
    }
}

int _len_to_print(int screen_no, int start_position) {
    return get_len(screen_no) - start_position;
}

void _refresh_screen(int screen_no) {
    text_char_t *text = get_text(screen_no);

    // invariant: never too many characters to write
    int start_writing_at = current_start_position[screen_no]; 

    int x = 0;
    int y = 0;

    for (int i = start_writing_at; i < get_len(screen_no); i++) {
        terminal_putentryat(text[i].c, text[i].color, x, y);
        if (++x == VGA_WIDTH) {
            x = 0;
            if (++y == VGA_HEIGHT)
                y = 0;
        }
    }
}

void screen_add_char(char c, uint8_t color, int screen_no) {
    text_char_t new_char;
    new_char.c = c;
    new_char.color = color;

    // Write character to text layer
    insert_character(new_char, current_index[screen_no], screen_no);

    current_index[screen_no] += 1;

    // If writing causes scroll
    if (_len_to_print(screen_no, current_start_position[screen_no]) > 2000) {
        current_start_position[screen_no] += 80;
    }

    // Else
    _refresh_screen(screen_no);

}