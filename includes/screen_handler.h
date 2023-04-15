#ifndef SCREEN_HANDLER_H
# define SCREEN_HANDLER_H

# include "direction.h"
# include "screen.h"


void screen_handle_scroll(direction dir, int screen_no);
void screen_add_char(char c, uint8_t color, int screen_no);
void screen_erase(int screen_no);


#endif