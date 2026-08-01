/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dtoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 19:49:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/01 21:35:00 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <math.h>
#include <stdlib.h>

static int	get_number_int_size(double number)
{
	size_t	int_size;
	int		int_nb;

	int_size = (number < 0.0);
	int_nb = (int)fabs(number);
	if (int_nb == 0)
		return (int_size + 1);
	while (int_nb)
	{
		int_size++;
		int_nb /= 10;
	}
	return (int_size);
}

static void	set_int_part(double number, size_t int_size, char *nb_str)
{
	int	i;
	int	int_nb;

	if (!nb_str)
		return ;
	if (number < 0)
		nb_str[0] = '-';
	i = int_size;
	int_nb = (int)fabs(number);
	while (i - (number < 0.0))
	{
		nb_str[i - 1] = int_nb % 10 + '0';
		i--;
		int_nb /= 10;
	}
}

static void	set_decimal_part(double number, size_t int_size, int precision,
	char *nb_str)
{
	int	i;

	i = 0;
	number = fabs(number - abs((int)number));
	while (i < precision)
	{
		number *= 10;
		nb_str[int_size + i + 1] = (int)number % 10 + '0';
		i++;
	}
}

char	*ft_dtoa(double number, int precision)
{
	size_t	int_size;
	char	*nb_str;

	int_size = get_number_int_size(number);
	nb_str = ft_calloc(int_size + precision + 2, sizeof(char));
	if (!nb_str)
		return (NULL);
	set_int_part(number, int_size, nb_str);
	nb_str[int_size] = '.';
	set_decimal_part(number, int_size, precision, nb_str);
	return (nb_str);
}
