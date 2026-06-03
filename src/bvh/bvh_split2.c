/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 14:19:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/03 16:57:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static t_vec3	half_nth_largest_axis(t_vec3 vec, int n)
{
	t_vec3	axis;

	axis = (t_vec3){3, 3, 3};
	if (vec.x > vec.y || vec.x > vec.z)
		axis.x = 2.0 - (vec.x > vec.y && vec.x > vec.z);
	if (vec.y > vec.x || vec.y > vec.z)
		axis.y = 2.0 - (vec.y > vec.x && vec.y > vec.z);
	if (vec.z > vec.x || vec.z > vec.y)
		axis.z = 2.0 - (vec.z > vec.x && vec.z > vec.y);
	if (fabs(axis.x - n) < 1e-6)
		vec.x /= 2.0;
	else if (fabs(axis.y - n) < 1e-6)
		vec.y /= 2.0;
	else
		vec.z /= 2.0;
	return (vec);
}

int	count_elements_split_right(t_bvh *bvh, t_vec3 split)
{
	int		i;
	int		count;
	t_vec3	center;

	i = bvh->first_index;
	split = vec3_add(split, bvh->aabb_min);
	count = 0;
	while (i < bvh->first_index + bvh->nb_elements)
	{
		center = get_object_center(bvh->objects[i]);
		if ((center.x > split.x) || (center.y > split.y)
			|| (center.z > split.z))
			count++;
		i++;
	}
	return (count);
}

t_vec3	get_range_from_object_centers(t_bvh *bvh)
{
	t_vec3	min;
	t_vec3	max;
	t_vec3	range;
	int		i;

	min = (t_vec3){1e30, 1e30, 1e30};
	max = (t_vec3){-1e30, -1e30, -1e30};
	i = bvh->first_index;
	while (i < bvh->first_index + bvh->nb_elements)
	{
		min = vec3_min(min, get_object_center(bvh->objects[i]));
		max = vec3_max(max, get_object_center(bvh->objects[i]));
		i++;
	}
	range = vec3_sub(max, min);
	return (range);
}

t_vec3	get_left_bounds(t_bvh *bvh)
{
	t_vec3	range;
	t_vec3	bound;
	int		count;

	range = vec3_sub(bvh->aabb_max, bvh->aabb_min);
	bound = half_nth_largest_axis(range, 1);
	count = count_elements_split_right(bvh, bound);
	if (count == 0 || count == bvh->nb_elements)
	{
		bound = half_nth_largest_axis(range, 2);
		count = count_elements_split_right(bvh, bound);
	}
	if (count == 0 || count == bvh->nb_elements)
	{
		bound = half_nth_largest_axis(range, 3);
		count = count_elements_split_right(bvh, bound);
	}
	if (count == 0 || count == bvh->nb_elements)
	{
		range = get_range_from_object_centers(bvh);
		bound = half_nth_largest_axis(range, 1);
	}
	return (bound);
}
