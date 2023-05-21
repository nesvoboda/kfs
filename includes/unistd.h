#ifndef FUNISTD_H
# define FUNISTD_H

# include "logs.h"
# include "types.h"
# include "fd.h"


#include "screen_handler.h"

void write(int fd, const void *buf, int count);
void test_func();

#endif