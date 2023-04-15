#ifndef TEXT_HANDLER_H
# define TEXT_HANDLER_H

#include "screen.h"
#include "config.h"

typedef struct text_char {
    char c;
    uint8_t color;
} text_char_t;

int insert_character(text_char_t c, int index, int screen_no);
int remove_character(int index, int screen_no);
text_char_t* get_text(int screen_no); 
int get_len(int screen_no);


#endif