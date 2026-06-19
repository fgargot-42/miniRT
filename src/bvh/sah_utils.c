/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 20:58:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/19 21:08:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

void	aabb_grow_to_include(t_aabb *aabb, t_object *obj)
{
	t_aabb	obj_box;

	get_object_aabb(obj, &obj_box);
	aabb->min = vec3_min(aabb->min, obj_box.min);
	aabb->max = vec3_max(aabb->max, obj_box.max);
}

void	aabb_grow_to_include_center(t_aabb *aabb, t_object *obj)
{
	t_vec3	center;

	center = get_object_center(obj);
	aabb->min = vec3_min(aabb->min, center);
	aabb->max = vec3_max(aabb->max, center);
}

double	get_aabb_area(t_aabb aabb)
{
	t_vec3	e;

	e = vec3_sub(aabb.max, aabb.min);
	return ((e.x * e.y + e.x * e.z + e.y * e.z));
}

void	sah_partition(t_bvh *node, t_sah *sah)
{
	int			i;
	int			j;

	if (sah->axis == -1 || !sah->count_l || !sah->count_r)
		return ;
	i = node->first_index;
	j = node->first_index + node->nb_elements - 1;
	while (i <= j)
	{
		while (i <= j && get_object_center(
				node->objects.array[i]).vec[sah->axis] < sah->pos)
			i++;
		while (i <= j && get_object_center(
				node->objects.array[j]).vec[sah->axis] >= sah->pos)
			j--;
		if (i < j)
			ft_memswap(&node->objects.array[i], &node->objects.array[j],
				sizeof(t_object *));
	}
	sah->count_l = i - node->first_index;
	sah->count_r = node->nb_elements - (i - node->first_index);
}

t_bin	*init_bins(int axis, t_aabb bounds)
{
	t_bin	*bins;
	int		index;

	index = 0;
	if (bounds.min.vec[axis] == bounds.max.vec[axis])
		return (NULL);
	bins = ft_calloc(SAH_BINS, sizeof(t_bin));
	if (!bins)
		return (NULL);
	while (index < SAH_BINS)
	{
		bins[index].aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
		bins[index].aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
		index++;
	}
	return (bins);
}
