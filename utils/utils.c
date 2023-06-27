#include "utils.h"

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memset(void* b, int c, int len)
{
	unsigned char* str;
	unsigned char* lim;

	str = b;
	lim = b + len;
	while (str != lim)
		*(str++) = (unsigned char)c;
	return (b);
}

int compatib(int a, int b)
{
	int diff;

	diff = a - b;
	if (diff < 0) {
		return diff;
	} else if (diff == 0) {
		return 0;
	} else {
		return diff;
	}
}

int ft_strncmp(const char* s1, const char* s2, size_t n)
{
	size_t i;
	unsigned char* u1;
	unsigned char* u2;

	u1 = (unsigned char*)s1;
	u2 = (unsigned char*)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (u1[i] != '\0' && u2[i] != '\0' && i < n && u1[i] == u2[i])
		i++;
	if (i == n)
		return (compatib(u1[i - 1], u2[i - 1]));
	return (compatib(u1[i], u2[i]));
}