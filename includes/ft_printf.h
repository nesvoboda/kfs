/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashishae <ashishae@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/25 17:45:01 by ashishae          #+#    #+#             */
/*   Updated: 2019/11/26 12:50:30 by ashishae         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
#define FT_PRINTF_H
#define CONVERSIONS "cspdiuxX%"
#define FLAGS "0-"
// #include <stdarg.h>
#include <stdlib.h>

#include "io.h"
#include "logs.h"
#include "screen_handler.h"

typedef struct s_info {
	char* flags;
	int width;
	char spacer;
	int minus;
	int precision;
	int precision_spaces;
	int width_spaces;
} t_info;

typedef __builtin_va_list va_list;
#define va_start(ap,last) __builtin_va_start(ap, last)
#define va_end(ap) __builtin_va_end(ap)
#define va_arg(ap,type) __builtin_va_arg(ap,type)
#define va_copy(dest, src) __builtin_va_copy(dest,src)


/*
**	Libft functions
*/
int ft_atoi(const char* str);
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char* s, int fd);
char* ft_substr(char const* s, unsigned int start, size_t len);
char* ft_strchr(const char* s, int c);
size_t ft_strlen(const char* str);

int hex_length(unsigned long long int n);
void print_hex(va_list *kwargs, int* ret, int capital, t_info info);
int put_spaces(int n, char spacer);
int print_string(va_list *kwargs, t_info info);
void print_pointer(va_list *kwargs, int* ret, t_info info);
void print_integer(va_list *kwargs, int* ret, t_info info);
void print_unsigned(va_list *kwargs, int* ret, t_info info);
int print_char(va_list *kwargs, t_info info);
int print_pc(t_info info);
void calculate_spaces(t_info* info, int length);
int printk(enum log_level level, const char* str, ...);
int printf(const char* str, ...);
int put_spaces(int n, char spacer);
void grab_flags(char** s, char* flags);
int grab_width(char** s, int* it, va_list *kwargs);

extern int current_fd;

#endif
