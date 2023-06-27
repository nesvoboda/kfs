#ifndef FUNISTD_H
#define FUNISTD_H

#include "cursor.h"
#include "fd.h"
#include "keyboard.h"
#include "logs.h"
#include "types.h"

#include "screen_handler.h"

void io_add_char(char c, uint8_t color);
void io_erase();

void write(int fd, const void* buf, int count);
void test_func();
int read(int fd, void* buf, int count);

#endif