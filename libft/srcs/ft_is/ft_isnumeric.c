/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumeric.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 20:56:10 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/12 00:59:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_numeric(char *nstr)
{
	int	has_decimal_part;
	int	has_digit;
	int	i;

	has_decimal_part = 0;
	has_digit = 0;
	i = 0;
	while (ft_iswhitespace(nstr[i]))
		i++;
	if (nstr[i] == '+' || nstr[i] == '-')
		i++;
	while (ft_isdigit(nstr[i]) || nstr[i] == '.')
	{
		if (nstr[i] == '.')
		{
			if (has_decimal_part)
				break ;
			else
				has_decimal_part = 1;
		}
		else
			has_digit = 1;
		i++;
	}
	return (has_digit);
}
