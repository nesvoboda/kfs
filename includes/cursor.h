#ifndef CURSOR_H
#define CURSOR_H
#include "idt.h"

void enable_cursor(u8int cursor_start, u8int cursor_end);
void disable_cursor();
void update_cursor(u16int pos);

#endif
