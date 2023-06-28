/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_char.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:47:47 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/25 13:47:52 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int print_pc(t_info info)
{
	int i;

	if (!info.minus)
		i = put_spaces(info.width - 1, info.spacer);
	ft_putchar_fd('%', current_fd);
	if (info.minus)
		i = put_spaces(info.width - 1, ' ');
	return (1 + i);
}

int print_char(va_list *kwargs, t_info info)
{
	int c;
	int i;

	c = va_arg(*kwargs, int);
	if (!info.minus)
		i = put_spaces(info.width - 1, info.spacer);
	ft_putchar_fd((unsigned char)c, current_fd);
	if (info.minus)
		i = put_spaces(info.width - 1, info.spacer);
	return (1 + i);
}
