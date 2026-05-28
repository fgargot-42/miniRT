/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 21:52:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 21:59:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

t_vec3	get_left_bounds(t_bvh *bvh)
{
	t_vec3	bound;

	bound = vec3_sub(bvh->aabb_max, bvh->aabb_min);
	if (bound.x > bound.y && bound.x > bound.z)
		bound.x /= 2.0;
	else if (bound.y > bound.z)
		bound.y /= 2.0;
	else
		bound.z /= 2.0;
/*#if DEBUG
	printf("aabb_min: x=% .3f y=% .3f z=% .3f\n",
		bvh->aabb_min.x, bvh->aabb_min.y, bvh->aabb_min.z);
	printf("aabb_max: x=% .3f y=% .3f z=% .3f\n",
		bvh->aabb_max.x, bvh->aabb_max.y, bvh->aabb_max.z);
	printf("left_bound: x=% .3f y=% .3f z=% .3f\n",
		bound.x, bound.y, bound.z);
#endif*/
	return (bound);
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
