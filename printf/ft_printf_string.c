/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 13:31:19 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/25 16:18:17 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int print_string(va_list kwargs, t_info info)
{
	char* str;
	int str_length;
	int spaces;

	str = va_arg(kwargs, char*);
	if (str == 0)
		str = "(null)";
	str_length = ft_strlen(str);
	if (info.precision > -1 && info.precision < str_length) {
		str_length = info.precision;
		spaces = info.width - info.precision;
	} else
		spaces = info.width - str_length;
	if (!info.minus)
		str_length += put_spaces(spaces, info.spacer);
	ft_putstr_fd(str, 1);
	if (info.minus)
		str_length += put_spaces(spaces, ' ');
	return (str_length);
}
