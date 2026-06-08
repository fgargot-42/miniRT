/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 19:57:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/08 20:48:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_array_sort(t_array arr, int (*cmp)(void*, void*))
{
	size_t	i;
	size_t	j;
	void	*tmp;

	i = 0;
	while (i < arr.len - 1)
	{
		j = i + 1;
		while (j < arr.len)
		{
			if (cmp(arr.array[i], arr.array[j]) > 0)
			{
				tmp = arr.array[i];
				arr.array[i] = arr.array[j];
				arr.array[j] = tmp;
			}
			j++;
		}
		i++;
	}
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
