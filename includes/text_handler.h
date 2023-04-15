#ifndef TEXT_HANDLER_H
# define TEXT_HANDLER_H

#include "screen.h"

typedef struct text_char {
    char c;
    uint8_t color;
} text_char_t;

void insert_character(text_char_t c, int index, int screen_no);
void erase(int index, int screen_no);
void remove_character(int index, int screen_no);
text_char_t* get_text(int screen_no); 
int get_len(int screen_no);


#endif