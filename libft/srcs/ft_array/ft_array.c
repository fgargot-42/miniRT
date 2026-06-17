/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_array.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/06 19:27:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/17 16:38:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

t_array	ft_arraynew(void)
{
	size_t	i;
	t_array	new_array;

	ft_bzero(&new_array, sizeof(t_array));
	i = 4096;
	new_array.array = ft_calloc(i, sizeof(void *));
	if (new_array.array)
	{
		new_array.size = i;
		new_array.len = 0;
	}
	return (new_array);
}

void	ft_arrayclear(t_array *arr, void (*del)(void*))
{
	size_t	i;

	i = 0;
	while (i < arr->len)
	{
		if (!del)
			break ;
		del(arr->array[i]);
		i++;
	}
	free(arr->array);
	arr->size = 0;
	arr->len = 0;
	arr->array = NULL;
}

void	ft_arrayadd_back(t_array *arr, void *new, void (*del)(void *))
{
	void	**new_alloc;
	size_t	new_size;

	new_alloc = NULL;
	new_size = arr->size;
	if (arr->len >= new_size)
	{
		while (arr->len >= new_size)
			new_size *= 2;
		new_alloc = ft_calloc(new_size, sizeof(void *));
		if (!new_alloc)
		{
			ft_arrayclear(arr, del);
			return ;
		}
		ft_memmove(new_alloc, arr->array, arr->size * sizeof(void *));
		free(arr->array);
		arr->array = new_alloc;
		arr->size = new_size;
	}
	arr->array[arr->len] = new;
	arr->len++;
}

ssize_t	ft_arrayget_index(t_array arr, void *elem)
{
	ssize_t	i;

	i = 0;
	while ((size_t)i < arr.len)
	{
		if (arr.array[i] == elem)
			return (i);
		i++;
	}
	return (-1);
}

t_array	ft_arraydup(t_array src, void (*dup)(void**, void*))
{
	t_array	dst;
	size_t	i;

	ft_bzero(&dst, sizeof(t_array));
	dst.array = ft_calloc(src.size, sizeof(void *));
	if (!dst.array)
		return (dst);
	dst.size = src.size;
	dst.len = src.len;
	i = 0;
	while (i < dst.len)
	{
		dup(&dst.array[i], src.array[i]);
		i++;
	}
	return (dst);
}
