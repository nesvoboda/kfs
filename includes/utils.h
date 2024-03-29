#ifndef UTILS_H
#define UTILS_H

#include "types.h"

size_t strlen(const char* str);
void* _memset(void* b, int c, int len);
void ft_putnbr(int n);
int ft_strncmp(const char* s1, const char* s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
u32int	ft_atoi_hex(const char *str);
char			**ft_split(char const *s, char c);
int ft_atoi(const char* str);

#endif
