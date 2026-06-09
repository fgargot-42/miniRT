/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:56:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/09 23:44:32 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static int	cmp_center(t_array a, int i, int j, char axis)
{
	t_vec3	center_i;
	t_vec3	center_j;

	center_i = get_object_center(a.array[i]);
	center_j = get_object_center(a.array[j]);
	if (axis == 'x')
		return (center_i.x < center_j.x);
	if (axis == 'y')
		return (center_i.y < center_j.y);
	return (center_i.z < center_j.z);
}

static void	merge(t_array a, t_array b, int *range, char axis)
{
	int		middle;
	int		i;
	int		j;
	int		k;

    middle = (range[0] + range[1]) / 2;
	i = range[0];
	j = middle;
	k = i;
    while (k < range[1])
	{
		if (i < middle && (j >= range[1] || cmp_center(b, i, j, axis)))
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

static void split_merge(t_array a, int begin, int end, t_array b, char axis)
{
	int	middle;
	int	range[2];

    if (end - begin <= 1) 
        return;
    middle = (begin + end) / 2;
    split_merge(b, begin, middle, a, axis);
    split_merge(b, middle, end, a, axis);
	range[0] = begin;
	range[1] = end;
	merge(a, b, range, axis);
}

void array_merge_sort(t_array obj, int start, int count, char axis)
{
	int		i;
	t_array	b;

	i = 0;
	b = ft_arraynew();
	while (i < (int)obj.len)
	{
		ft_arrayadd_back(&b, obj.array[i], free_object);
		i++;
	}
    split_merge(obj, start, start + count, b, axis);
	free(b.array);
}
