#include "utils.h"

size_t strlen(const char* str)
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* _memset(void* b, int c, int len)
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

void* memcpy(void* dst, const void* src, size_t n)
{
	size_t i;
	unsigned char* our_dst;
	unsigned char* our_src;

	i = 0;
	our_dst = dst;
	our_src = (unsigned char*)src;
	if (dst == NULL && src == NULL)
		return (NULL);
	while (i < n) {
		our_dst[i] = our_src[i];
		i++;
	}
	return (dst);
}

int to_dec(char hex) {
	if (hex >= '0' && hex <= '9') {
		return hex - '0';
	}
	if (hex >= 'A' && hex <= 'Z') {
		return hex - 'A' + 10;
	}
	return hex - 'a' + 10;
}

int is_num(char c) {
	return c >= '0' && c <= '9';
}

int is_hex(char c) {
	return is_num(c) || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// No sign for now
u32int	ft_atoi_hex(const char *str)
{
	int		nbr;

	nbr = 0;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
			|| (*str) == '\r' || (*str) == ' ')
		str++;
	while ((*str) != '\0' && is_hex(*str))
	{
		nbr *= 16;
		nbr += to_dec(*str);
		str++;
	}
	return (nbr);
}

int ft_atoi(const char* str)
{
	int nbr;
	int sign;

	nbr = 0;
	sign = 1;
	while ((*str) == '\t' || (*str) == '\n' || (*str) == '\v' || (*str) == '\f'
		|| (*str) == '\r' || (*str) == ' ')
		str++;
	if ((*str) == '-' || (*str) == '+') {
		sign *= ((*str) == '-' ? -1 : 1);
		str++;
	}
	while ((*str) != '\0' && (*str) >= '0' && (*str) <= '9') {
		nbr *= 10;
		nbr += (*str) - '0';
		str++;
	}
	return (nbr * sign);
}
