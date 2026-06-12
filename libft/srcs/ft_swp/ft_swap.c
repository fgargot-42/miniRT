/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   swap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 14:55:55 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/12 15:05:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

void	ft_cswap(char *a, char *b)
{
	char	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_iswap(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_fswap(float *a, float *b)
{
	float	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_dswap(double *a, double *b)
{
	double	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_memswap(void *a, void *b, size_t size)
{
	void	*tmp;

	tmp = ft_calloc(1, size);
	if (!tmp)
		return ;
	ft_memcpy(tmp, a, size);
	ft_memcpy(a, b, size);
	ft_memcpy(b, tmp, size);
	free(tmp);
}
