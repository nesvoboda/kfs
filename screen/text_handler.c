#include "text_handler.h"

// #define SCREEN_MAX 5


#define MAX_TEXT 3000

typedef struct text_buffer {
    text_char_t buf[MAX_TEXT];
    int len;
} text_buffer_t;

text_buffer_t buffers[SCREEN_MAX];

void init_text_handler() {
    for (int i = 0; i < SCREEN_MAX; i++) {
        // memset(buffers[i].buf, 0, MAX_TEXT);
        buffers[i].len = 0;
    }
}

// abc
// insert 'x' at index 1
// axbc


// abbc


void _move_everything_forward(int at_index, int screen_no)
{
    text_char_t *buf = buffers[screen_no].buf;

    text_char_t save = buf[at_index];
    text_char_t second_save = buf[at_index]; 
    for (int i = at_index+1; i < buffers[screen_no].len + 1; i++) {
        second_save = buf[i];
        buf[i] = save;
        save = second_save;
    }
}

// abc
// ac

// abcd
// acdd

void _move_everything_back(int at_index, int screen_no)
{
    text_char_t *buf = buffers[screen_no].buf;

    for (int i = at_index; i < buffers[screen_no].len - 1; i++) {
        buf[i] = buf[i+1];
    }
    buf[buffers[screen_no].len - 1].c = '\0';
}

int insert_character(text_char_t c, int index, int screen_no) {
    if (buffers[screen_no].len == MAX_TEXT) {
        return -1;
    }
    
    _move_everything_forward(index, screen_no);
    buffers[screen_no].buf[index] = c;
    buffers[screen_no].len += 1;
    return 0;
}

// abc
// erase at 1
// ac


int remove_character(int index, int screen_no)
{
    if (buffers[screen_no].len == 0) {
        return -1;
    }

    _move_everything_back(index, screen_no);
    buffers[screen_no].len -= 1;
    return 0;
}

text_char_t* get_text(int screen_no) {
    return buffers[screen_no].buf;
}

int get_len(int screen_no) {
    return buffers[screen_no].len;
}
