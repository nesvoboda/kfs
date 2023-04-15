#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

size_t strlen(const char* str);
void	*memset(void *b, int c, int len);
void	ft_putnbr(int n);


#endif