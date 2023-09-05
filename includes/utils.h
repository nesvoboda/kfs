#ifndef UTILS_H
#define UTILS_H

#include "types.h"

size_t strlen(const char* str);
void* memset(void* b, int c, int len);
void ft_putnbr(int n);
int ft_strncmp(const char* s1, const char* s2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);


#endif