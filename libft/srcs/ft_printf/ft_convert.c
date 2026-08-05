/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_convert.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:56:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/05 19:01:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include "libft.h"

int	ft_print_c(va_list args)
{
	char	c;

	c = va_arg(args, int);
	return (write(1, &c, 1));
}

int	ft_print_s(va_list args)
{
	char	*s;

	s = va_arg(args, char *);
	if (!s)
		return (write(1, "(null)", 6));
	return (write(1, s, ft_strlen(s)));
}

int	ft_print_p(va_list args)
{
	void	*p;
	int		len;
	int		i;
	char	*res;

	p = va_arg(args, void *);
	if (!p)
		return (write(1, "(nil)", 5));
	len = write(1, "0x", 2);
	res = ft_ptrtoa(p);
	i = 0;
	while (res[i] && res[i] == '0')
		i++;
	len += write(1, &res[i], ft_strlen(&res[i]));
	free(res);
	return (len);
}
