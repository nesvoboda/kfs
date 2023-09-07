/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/21 15:32:23 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 17:56:37 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int conversion(char* str, va_list* kwargs, int* ret, t_info info)
{
	int move;

	move = 1;
	if (str[0] == 'c')
		*ret += print_char(kwargs, info);
	else if (str[0] == 's')
		(*ret) += print_string(kwargs, info);
	else if (str[0] == 'p')
		print_pointer(kwargs, ret, info);
	else if (str[0] == 'd')
		print_integer(kwargs, ret, info);
	else if (str[0] == 'i')
		print_integer(kwargs, ret, info);
	else if (str[0] == 'u')
		print_unsigned(kwargs, ret, info);
	else if (str[0] == 'x')
		print_hex(kwargs, ret, 0, info);
	else if (str[0] == 'X')
		print_hex(kwargs, ret, 1, info);
	else if (str[0] == '%')
		*ret += print_pc(info);
	else
		move = 0;
	return (move);
}

void format_handler(char* str, va_list* kwargs, int* ret, int* i)
{
	char flags[100];
	t_info info;

	str++;
	grab_flags(&str, flags);
	info.flags = flags;
	info.width = grab_width(&str, i, kwargs);
	info.spacer = ft_strchr(info.flags, '0') ? '0' : ' ';
	info.minus = ft_strchr(info.flags, '-') ? 1 : 0;
	if (info.width < 0) {
		info.width = -info.width;
		info.minus = 1;
	}
	if (*str == '.') {
		str++;
		(*i)++;
		info.precision = grab_width(&(str), i, kwargs);
	} else
		info.precision = -1;
	*i += conversion(str, kwargs, ret, info);
	*i += ft_strlen(flags);
}

int printk(enum log_level level, const char* str, ...)
{
	current_fd = KERNEL;
	int i;
	int ret;
	va_list kwargs;

	start_log(level);
	i = 0;
	ret = 0;
	if (str == 0)
		return (0);
	va_start(kwargs, str);
	while (str[i]) {
		if (str[i] == '%') {
			format_handler((char*)str + i, &kwargs, &ret, &i);
			i++;
		} else {
			ft_putchar_fd(str[i], current_fd);
			i++;
			ret++;
		}
	}
	va_end(kwargs);
	end_log();
	refresh_logs();
	return (ret);
}

int current_fd = -1;

int printf(const char* str, ...)
{
	current_fd = STDOUT;
	int i;
	int ret;
	va_list kwargs;

	i = 0;
	ret = 0;
	if (str == 0)
		return (0);
	va_start(kwargs, str);
	while (str[i]) {
		if (str[i] == '%') {
			format_handler((char*)str + i, &kwargs, &ret, &i);
			i++;
		} else {
			ft_putchar_fd(str[i], current_fd);
			i++;
			ret++;
		}
	}
	va_end(kwargs);
	return (ret);
}
