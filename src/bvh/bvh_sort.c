/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_sort.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:56:49 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/10 21:42:59 by fgargot          ###   ########.fr       */
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

static void	split_merge(t_array a, int *range, t_array b, char axis)
{
	int	middle;
	int	srange[2];

	if (range[1] - range[0] <= 1)
		return ;
	middle = (range[0] + range[1]) / 2;
	srange[0] = range[0];
	srange[1] = middle;
	split_merge(b, srange, a, axis);
	srange[0] = middle;
	srange[1] = range[1];
	split_merge(b, srange, a, axis);
	merge(a, b, range, axis);
}

void	array_merge_sort(t_array obj, int start, int count, char axis)
{
	int		i;
	t_array	b;
	int		range[2];

	i = 0;
	b = ft_arraynew();
	while (i < (int)obj.len)
	{
		ft_arrayadd_back(&b, obj.array[i], free_object);
		i++;
	}
	range[0] = start;
	range[1] = start + count;
	split_merge(obj, range, b, axis);
	free(b.array);
}
