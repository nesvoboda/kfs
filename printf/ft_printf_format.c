/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:54:37 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 12:46:25 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int put_spaces(int n, char spacer)
{
	int i;

	i = 0;
	while (i < n) {
		ft_putchar_fd(spacer, current_fd);
		i++;
	}
	return (i);
}

void grab_flags(char** s, char* flags)
{
	int i;
	int y;
	char* str;

	str = *s;
	i = 0;
	y = 0;
	while (ft_strchr(FLAGS, *str) != NULL && *str)
		flags[y++] = *(str++);
	flags[y] = '\0';
	*s = str;
}

int grab_width(char** s, int* it, va_list kwargs)
{
	char* str;
	char number[11];
	int i;
	int y;

	str = *s;
	i = 0;
	y = 0;
	while (str[i] >= '0' && str[i] <= '9')
		number[y++] = str[i++];
	if (str[i] == '*') {
		*it += 1;
		*s += 1;
		return (va_arg(kwargs, int));
	}
	*it += i;
	*s += i;
	number[y] = '\0';
	y = ft_atoi(number);
	return (y);
}
