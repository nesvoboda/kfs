/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:39:21 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/28 10:22:33 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		hex_length(unsigned long long int n)
{
	int i;

	if (n == 0)
		return (1);
	i = 0;
	while (n != 0)
	{
		n = n / 16;
		i++;
	}
	return (i);
}

void	pthex(unsigned int nbr, int *ret, int capital)
{
	char			*base;

	if (capital == 1)
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (nbr > 15)
	{
		pthex(nbr / 16, ret, capital);
		pthex(nbr % 16, ret, capital);
	}
	if (nbr <= 15)
	{
		ft_putchar_fd(base[nbr], KERNEL);
		(*ret)++;
	}
}

void	print_hex(va_list kwargs, int *ret, int capital, t_info info)
{
	unsigned int	nbr;
	int				length;

	nbr = va_arg(kwargs, unsigned int);
	length = hex_length(nbr);
	calculate_spaces(&info, length);
	if (info.precision == 0 && nbr == 0)
		info.width_spaces += 1;
	if (!info.minus)
		*ret += put_spaces(info.width_spaces, info.spacer);
	if (info.precision != -1 && info.precision > length)
		*ret += put_spaces(info.precision_spaces, '0');
	if (!(info.precision == 0 && nbr == 0))
		pthex(nbr, ret, capital);
	if (info.minus)
		*ret += put_spaces(info.width_spaces, ' ');
}
