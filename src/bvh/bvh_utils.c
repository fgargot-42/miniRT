/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 21:52:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/17 23:35:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
//
//void	array_merge_sort(t_array obj, int start, int count, char axis);
//
//void	sort_bvh_objects(t_bvh *bvh, int axis)
//{
//	if (axis == 0)
//		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
//			bvh->nb_elements, 'x');
//	if (axis == 1)
//		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
//			bvh->nb_elements, 'y');
//	if (axis == 2)
//		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
//			bvh->nb_elements, 'z');
//}

t_vec3	get_object_center(t_object *obj)
{
	t_aabb	aabb;

	aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
	aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
	if (obj->type != OBJ_PARABOLOID && obj->type != OBJ_TRIANGLE)
		return (obj->position);
	get_object_aabb(obj, &aabb);
	return (vec3_scale(vec3_add(aabb.min, aabb.max), 1.0 / 2.0));
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

//void	sort_bvh_objects_asc(t_array array, int min, int count, char axis)
//{
//	array_merge_sort(array, min, count, axis);
//}
