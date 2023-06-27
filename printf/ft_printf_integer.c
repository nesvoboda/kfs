/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_integer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:42:10 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 17:40:21 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void ptnbr(int n, int* ret)
{
	int nbr;

	if (n < 0) {
		nbr = (n == -2147483648) ? 2147483647 : -n;
	} else
		nbr = n;
	if (nbr > 9) {
		ptnbr(nbr / 10, ret);
		ptnbr(nbr % 10, ret);
	}
	if (nbr <= 9) {
		ft_putchar_fd(nbr + '0', KERNEL);
		(*ret)++;
	}
}

int int_length(int n)
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

void calculate_spaces(t_info* info, int length)
{
	if (info->precision > length)
		info->precision_spaces = info->precision - length;
	else
		info->precision_spaces = 0;
	info->width_spaces = info->width - (length + info->precision_spaces);
	if (info->precision > -1)
		info->spacer = ' ';
}

void print_integer(va_list kwargs, int* ret, t_info info)
{
	int nbr;
	int length;

	nbr = va_arg(kwargs, int);
	length = int_length(nbr);
	calculate_spaces(&info, length);
	if (nbr < 0) {
		(*ret)++;
		info.width_spaces -= 1;
	}
	if (info.precision == 0 && nbr == 0)
		info.width_spaces += 1;
	if (nbr < 0 && info.spacer == '0')
		ft_putchar_fd('-', KERNEL);
	if (!info.minus)
		*ret += put_spaces(info.width_spaces, info.spacer);
	if (nbr < 0 && info.spacer != '0')
		ft_putchar_fd('-', KERNEL);
	if (info.precision > length && info.precision != 0)
		*ret += put_spaces(info.precision_spaces, '0');
	if (!(info.precision == 0 && nbr == 0))
		ptnbr(nbr, ret);
	if (info.minus)
		*ret += put_spaces(info.width_spaces, ' ');
}
