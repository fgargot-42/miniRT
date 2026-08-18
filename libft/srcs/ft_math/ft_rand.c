/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_rand.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 17:50:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/18 21:30:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>

static int	next_rand(int seed, bool init)
{
	static int			val = 0;
	static int			prev = 0;
	static const int	mult = 1103515245;
	static const int	add = 12345;

	prev = val;
	if (init)
		val = seed;
	else
		val = val * mult + add;
	return ((prev << 16 | (val & 0xffff)) & FT_RANDMAX);
}

int	ft_rand(void)
{
	return (next_rand(0, false));
}

void	ft_srand(int seed)
{
	next_rand(seed, true);
}

int	ft_rand_r(int *state)
{
	int					prev;
	static const int	mult = 1103515245;
	static const int	add = 12345;

	prev = *state;
	*state = *state * mult + add;
	return ((prev << 16 | (*state & 0xffff)) & FT_RANDMAX);
}
