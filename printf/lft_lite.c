/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lft_lite.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:45:28 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/25 18:03:47 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"

size_t ft_strlen(const char* str)
{
	char* start;

	start = (char*)str;
	while (*str)
		str++;
	return ((size_t)(str - start));
}

char* ft_strchr(const char* s, int c)
{
	int i;

	i = 0;
	while (s[i] != '\0') {
		if (s[i] == c)
			return ((char*)&s[i]);
		i++;
	}
	if (c == '\0')
		return ((char*)(&s[i]));
	return (NULL);
}
