/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:19:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:08:16 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "bvh.h"
#include "veclib.h"

void	count_elements_split_right(t_bvh *bvh, t_sah *sah)
{
	int		i;
	int		count;
	t_vec3	center;

	i = bvh->first_index;
	count = 0;
	while (i < bvh->first_index + bvh->nb_elements)
	{
		center = get_object_center(bvh->objects.array[i]);
		if (center.vec[sah->axis] > sah->pos)
			count++;
		i++;
	}
	sah->count_l = bvh->nb_elements - count;
	sah->count_r = count;
}

t_vec3	get_range_from_object_centers(t_bvh *bvh)
{
	t_vec3	min;
	t_vec3	max;
	t_vec3	range;
	int		i;

	min = (t_vec3){{1e30, 1e30, 1e30}};
	max = (t_vec3){{-1e30, -1e30, -1e30}};
	i = bvh->first_index;
	while (i < bvh->first_index + bvh->nb_elements)
	{
		min = vec3_min(min, get_object_center(bvh->objects.array[i]));
		max = vec3_max(max, get_object_center(bvh->objects.array[i]));
		i++;
	}
	range = vec3_sub(max, min);
	return (range);
}
