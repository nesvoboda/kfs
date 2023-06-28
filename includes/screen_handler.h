#ifndef SCREEN_HANDLER_H
#define SCREEN_HANDLER_H

#include "config.h"
#include "direction.h"
#include "logs.h"
#include "screen.h"
#include "text_handler.h"

// The line at which the text display starts
#define START_LINE 2
#define END_LINE 15
#define TEXT_AREA_LINES (END_LINE - START_LINE)
#define TEXT_AREA_SIZE TEXT_AREA_LINES* VGA_WIDTH

// void screen_handle_scroll(enum direction dir);
int screen_add_char(char c, uint8_t color);
void screen_erase();
void init_screen();
void refresh_screen();
void refresh_logs();

#endif