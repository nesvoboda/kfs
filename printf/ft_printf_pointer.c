/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:36:14 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 17:54:55 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void ptptr(unsigned long long int nbr, int* ret)
{
	char* base;

	base = "0123456789abcdef";
	if (nbr > 15) {
		ptptr(nbr / 16, ret);
		ptptr(nbr % 16, ret);
	}
	if (nbr <= 15) {
		ft_putchar_fd(base[nbr], KERNEL);
		*ret += 1;
	}
}

void print_pointer(va_list kwargs, int* ret, t_info info)
{
	unsigned long long int nbr;
	void* ptr;
	int length;

	ptr = va_arg(kwargs, void*);
	nbr = (unsigned long long int)ptr;
	length = hex_length(nbr) + 2;
	calculate_spaces(&info, length);
	if (info.precision == 0 && nbr == 0)
		info.width_spaces += 1;
	if (info.precision_spaces != 0)
		info.precision_spaces += 2;
	if (!info.minus)
		*ret += put_spaces(info.width_spaces, info.spacer);
	*ret += 2;
	ft_putstr_fd("0x", 1);
	if (info.precision > length && info.precision != 0)
		*ret += put_spaces(info.precision_spaces, '0');
	if (!(info.precision == 0 && ptr == 0))
		ptptr(nbr, ret);
	if (info.minus)
		*ret += put_spaces(info.width_spaces, ' ');
}
