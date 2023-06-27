#ifndef CURSOR_H
#define CURSOR_H
#include "idt.h"

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();
void update_cursor(uint16_t pos);

#endif