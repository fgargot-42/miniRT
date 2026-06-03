/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 21:52:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/03 14:19:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

void	sort_bvh_objects(t_bvh *bvh, t_vec3 left_bound)
{
	if (bvh->left && bvh->right && left_bound.x < bvh->aabb_max.x)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'x');
	if (bvh->left && bvh->right && left_bound.y < bvh->aabb_max.y)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'y');
	if (bvh->left && bvh->right && left_bound.z < bvh->aabb_max.z)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'z');
}

t_vec3	get_object_center(t_object *obj)
{
	t_vec3	aabb_min;
	t_vec3	aabb_max;

	aabb_min = (t_vec3){-1e30, -1e30, -1e30};
	aabb_max = (t_vec3){1e30, 1e30, 1e30};
	if (obj->type != OBJ_PARABOLOID && obj->type != OBJ_TRIANGLE)
		return (obj->position);
	if (obj->type == OBJ_PARABOLOID)
		get_paraboloid_aabb(obj, &aabb_min, &aabb_max);
	else if (obj->type == OBJ_TRIANGLE)
		get_triangle_aabb(obj, &aabb_min, &aabb_max);
	return (vec3_scale(vec3_add(aabb_min, aabb_max), 1.0 / 2.0));
}

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

void	sort_bvh_objects_asc(t_object **array, int min, int count, char axis)
{
	int			i;
	int			j;
	t_object	*swp;
	t_vec3		center[2];

	i = min;
	while (i < min + count - 1)
	{
		j = i + 1;
		while (j < min + count)
		{
			center[0] = get_object_center(array[i]);
			center[1] = get_object_center(array[j]);
			if ((axis == 'x' && center[0].x > center[1].x)
				|| (axis == 'y' && center[0].y > center[1].y)
				|| (axis == 'z' && center[0].z > center[1].z))
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
