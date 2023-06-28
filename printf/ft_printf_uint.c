/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_uint.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:45:48 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 17:54:22 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int uint_length(unsigned int n)
{
	int i;

	i = 0;
	if (n == 0)
		return (1);
	while (n != 0) {
		n = n / 10;
		i++;
	}
	return (i);
}

void ptunsigned(unsigned int n, int* ret)
{
	if (n > 9) {
		ptunsigned(n / 10, ret);
		ptunsigned(n % 10, ret);
	}
	if (n <= 9) {
		ft_putchar_fd(n + '0', current_fd);
		(*ret)++;
	}
}

void print_unsigned(va_list *kwargs, int* ret, t_info info)
{
	unsigned int nbr;
	int length;

	nbr = va_arg(*kwargs, unsigned int);
	length = uint_length(nbr);
	calculate_spaces(&info, length);
	if (info.precision == 0 && nbr == 0)
		info.width_spaces += 1;
	if (!info.minus)
		*ret += put_spaces(info.width_spaces, info.spacer);
	if (info.precision > length && info.precision != 0)
		*ret += put_spaces(info.precision_spaces, '0');
	if (!(info.precision == 0 && nbr == 0))
		ptunsigned(nbr, ret);
	if (info.minus)
		*ret += put_spaces(info.width_spaces, ' ');
}
