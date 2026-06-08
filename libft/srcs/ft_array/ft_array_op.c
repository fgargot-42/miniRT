/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array_op.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 19:57:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/08 15:52:41 by fgargot          ###   ########.fr       */
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
