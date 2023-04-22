#include "utils.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}


void	*memset(void *b, int c, int len)
{
	unsigned char	*str;
	unsigned char	*lim;

	str = b;
	lim = b + len;
	while (str != lim)
		*(str++) = (unsigned char)c;
	return (b);
}
