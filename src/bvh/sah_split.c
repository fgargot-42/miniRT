/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:04:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/11 20:51:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static void	aabb_grow_to_include(t_vec3 *min, t_vec3 *max, t_object *obj)
{
	t_aabb	obj_box;

	get_triangle_aabb(obj, &obj_box.min, &obj_box.max);
	*min = vec3_min(*min, obj_box.min);
	*max = vec3_max(*max, obj_box.max);
}

static double	get_area(t_vec3 a, t_vec3 b)
{
	t_vec3	e;

	e = vec3_sub(b, a);
	return (e.x * e.y + e.x * e.z + e.y * e.z);
}

static void	update_best_sah_split(t_vec3 cost, t_sah *sah, t_vec3 center)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (cost.vec[i] < sah->cost)
		{
			sah->cost = cost.vec[i];
			sah->axis = i;
			sah->pos = center.vec[i];
		}
		i++;
	}
}

static void	evaluate_sah_cost(t_bvh *node, t_vec3 pos, t_sah *sah)
{
	t_vec3	cost;
	t_vec3	aabb[2][2];
	t_vec3	center;
	int		count[2];
	int		i;
	int		j;

	i = 0;
	while (i < 3)
	{
		ft_bzero(&count, sizeof(int) * 2);
		aabb[0][0] = vec3_scale((t_vec3){{1, 1, 1}}, 1e30);
		aabb[0][1] = vec3_scale((t_vec3){{1, 1, 1}}, -1e30);
		ft_memcpy(&aabb[1], &aabb[0], 2 * sizeof(t_vec3));
		j = node->first_index;
		while (j < node->first_index + node->nb_elements)
		{
			center = get_object_center(node->objects.array[j]);
			if ((center.vec[i]) < pos.vec[i])
			{
				count[0]++;
				aabb_grow_to_include(&aabb[0][0], &aabb[0][1], node->objects.array[j]);
			}
			else
			{
				count[1]++;
				aabb_grow_to_include(&aabb[1][0], &aabb[1][1], node->objects.array[j]);
			}
			j++;
		}
		cost.vec[i] = count[0] * get_area(aabb[0][0], aabb[0][1])
			+ count[1] * get_area(aabb[1][0], aabb[1][1]);
		i++;
	}
	update_best_sah_split(cost, sah, pos);
}

t_sah	get_sah_split(t_bvh *node)
{
	int		i;
	t_vec3	center;
	t_sah	sah;
	double	parent_cost;

	i = node->first_index;
	sah.axis = 0;
	sah.pos = 0;
	sah.count_l = 0;
	sah.count_r = 0;
	sah.cost = 1e30;
	parent_cost = node->nb_elements * get_area(node->aabb.min, node->aabb.max);
	while (i < node->first_index + node->nb_elements)
	{
		center = get_object_center(node->objects.array[i]); 
		evaluate_sah_cost(node, center, &sah);
		i++;
	}
	if (sah.cost >= parent_cost)
		sah.axis = -1;
	return (sah);
}
