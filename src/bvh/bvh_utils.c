/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 21:52:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/14 00:35:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

int	is_bvh_object(void *e)
{
	int	is_bvh;

	is_bvh = ((t_object *)e)->type == OBJ_SPHERE;
	is_bvh |= ((t_object *)e)->type == OBJ_CYLINDER;
	is_bvh |= ((t_object *)e)->type == OBJ_CONE;
	is_bvh |= ((t_object *)e)->type == OBJ_HYPERBOLOID;
	is_bvh |= ((t_object *)e)->type == OBJ_PARABOLOID;
	is_bvh |= ((t_object *)e)->type == OBJ_TRIANGLE;
	return (is_bvh);
}

void	bvh_grow_to_include(t_bvh *bvh, t_object *object)
{
	t_vec3	aabb_min;
	t_vec3	aabb_max;

	static const t_obj_aabb_fn	get_aabb[] = {NULL, NULL, NULL, NULL, NULL,
		get_sphere_aabb, get_cylinder_aabb, get_cone_aabb,
		get_hyperboloid_aabb, get_paraboloid_aabb, get_triangle_aabb};
	aabb_min = bvh->aabb_min;
	aabb_max = bvh->aabb_max;
	if (object->type >= OBJ_SPHERE)
		get_aabb[object->type](object, &aabb_min, &aabb_max);
	if (bvh->aabb_min.x == 0 && bvh->aabb_min.y == 0 && bvh->aabb_min.z == 0
		&& bvh->aabb_max.x == 0 && bvh->aabb_max.y == 0 && bvh->aabb_max.z == 0)
	{
		bvh->aabb_min = aabb_min;
		bvh->aabb_max = aabb_max;
	}
	else
	{
		bvh->aabb_min = vec3_min(bvh->aabb_min, aabb_min);
		bvh->aabb_max = vec3_max(bvh->aabb_max, aabb_max);
	}
}

void	sort_bvh_objects_asc(t_object **array, int min, int max, char axis)
{
	int			i;
	int			j;
	t_object	*swp;

	i = min;
	while (i < max - 1)
	{
		j = i + 1;
		while (j < max)
		{
			if ((axis == 'x' && array[i]->position.x > array[j]->position.x)
				|| (axis == 'y' && array[i]->position.y > array[j]->position.y)
				|| (axis == 'z' && array[i]->position.z > array[j]->position.z))
			{
				swp = array[j];
				array[j] = array[i];
				array[i] = swp;
			}
			j++;
		}
		i++;
	}
}
