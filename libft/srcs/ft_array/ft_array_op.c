/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 19:57:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/09 23:59:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

static void	merge(t_array a, t_array b, int *range, int (*cmp)(void*, void*))
{
	int	middle;
	int	i;
	int	j;
	int	k;

	middle = (range[0] + range[1]) / 2;
	i = range[0];
	j = middle;
	k = i;
	while (k < range[1])
	{
		if (i < middle && (j >= range[1] || cmp(b.array[i], b.array[j]) <= 0))
		{
			a.array[k] = b.array[i];
			i++;
			k++;
			continue ;
		}
		a.array[k] = b.array[j];
		j++;
		k++;
	}
}

static void	split_merge(t_array a, int *range, t_array b,
		int (*cmp)(void*, void*))
{
	int	middle;
	int	srange[2];

	if (range[1] - range[0] <= 1)
		return ;
	middle = (range[0] + range[1]) / 2;
	srange[0] = range[0];
	srange[1] = middle;
	split_merge(b, srange, a, cmp);
	srange[0] = middle;
	srange[1] = range[1];
	split_merge(b, srange, a, cmp);
	srange[0] = range[0];
	merge(a, b, srange, cmp);
}

void	ft_array_sort(t_array arr, int (*cmp)(void*, void*), void (*del)(void*))
{
	size_t	i;
	t_array	b;
	int		range[2];

	i = 0;
	b = ft_arraynew();
	while (i < arr.len)
	{
		ft_arrayadd_back(&b, arr.array[i], del);
		i++;
	}
	range[0] = 0;
	range[1] = arr.len;
	split_merge(arr, range, b, cmp);
	free(b.array);
}

t_array	ft_array_map(t_array arr, void *(*f)(void *), void (*del)(void *))
{
	t_array	new_a;
	size_t	i;

	new_a = ft_arraynew();
	if (!new_a.array)
		return (new_a);
	i = 0;
	while (i < arr.len)
	{
		ft_arrayadd_back(&new_a, f(arr.array[i]), del);
		i++;
	}
	return (new_a);
}

t_array	ft_array_filter(t_array arr, int (*f)(void *), void (*del)(void *))
{
	t_array	new_a;
	size_t	i;

	new_a = ft_arraynew();
	if (!new_a.array)
		return (new_a);
	i = 0;
	while (i < arr.len)
	{
		if (f(arr.array[i]))
			ft_arrayadd_back(&new_a, arr.array[i], del);
		i++;
	}
	return (new_a);
}
