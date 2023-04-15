#ifndef TEXT_HANDLER_H
# define TEXT_HANDLER_H

typedef struct text_char {
    char c;
    uint8_t color;
} text_char_t;

void insert_character(text_char_t c, int index, int screen_no)
void erase(int index, int screen_no);
void remove_character(int index, int screen_no);
text_char_t* get_text(screen_no); 
int get_len();


#endif