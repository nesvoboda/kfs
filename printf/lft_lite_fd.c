/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_lite_fd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:51:20 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/25 17:57:46 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

enum DESCRIPTORS {
	KERNEL = 1,
};

void write(int fd, char *c, int len) {
	if (fd == KERNEL) {
		for (int i = 0; i < len; i++) {
			write_log(c[i]);
		}
	}
	else {
		// We can only write() to logs now...
		return;
	}
}

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (s != NULL)
		write(fd, s, ft_strlen(s));
}
