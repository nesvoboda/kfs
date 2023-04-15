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

void	ft_putnbr(int n)
{
	int nbr;

	if (n < 0)
	{
		terminal_putchar('-');
		nbr = (n == -2147483648) ? 2147483648 : -n;
	}
	else
		nbr = n;
	if (nbr > 9)
	{
		ft_putnbr(nbr / 10);
		ft_putnbr(nbr % 10);
	}
	if (nbr <= 9)
	{
		terminal_putchar(nbr + '0');
	}
}