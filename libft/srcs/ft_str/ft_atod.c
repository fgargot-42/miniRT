/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atod.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 19:18:43 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/11 20:21:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	long	get_nb(const char *nb_str, int *nb_digits_right)
{
	int		i;
	long	nb;

	i = 0;
	nb = 0;
	while (ft_isdigit(nb_str[i]))
	{
		nb = 10 * nb + nb_str[i] - '0';
		i++;
	}
	if (nb_str[i] == '.')
	{
		i++;
		while (ft_isdigit(nb_str[i]))
		{
			nb = 10 * nb + nb_str[i] - '0';
			(*nb_digits_right)++;
			i++;
		}
	}
	return (nb);
}

static long	ft_pow(int nb, int e)
{
	int	res;

	res = 1;
	if (e < 0)
		return (0);
	while (e)
	{
		res *= nb;
		e--;
	}
	return (res);
}

double	ft_atod(const char *nptr)
{
	int		nb_digits_right;
	int		sign;
	int		i;
	long	nb;
	long	div;

	nb_digits_right = 0;
	sign = 1;
	i = 0;
	while (ft_iswhitespace(nptr[i]))
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1;
		i++;
	}
	nb = get_nb(&nptr[i], &nb_digits_right);
	div = ft_pow(10, nb_digits_right);
	if (div != 0)
		return ((double)sign * (double)nb / (double)div);
	return ((double)sign * (double)nb);
}
