/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:27:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/16 20:53:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Fuctions to get aabb (axis-aligned bounding box) for each object

#include "miniRT.h"
#include "veclib.h"

void	get_sphere_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	extent;

	extent = vec3_scale((t_vec3){1, 1, 1}, obj->radius);
	*aabb_min = vec3_sub(obj->position, extent);
	*aabb_max = vec3_add(obj->position, extent);
}

void	get_cylinder_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	extent;

	if (!aabb_min || !aabb_max)
		return ;
	 extent.x = obj->props.height / 2.0 * fabs(obj->direction.x) + obj->radius *
	 	sqrt(1 - pow(obj->direction.x, 2));
	 extent.y = obj->props.height / 2.0 * fabs(obj->direction.y) + obj->radius *
	 	sqrt(1 - pow(obj->direction.y, 2));
	 extent.z = obj->props.height / 2.0 * fabs(obj->direction.z) + obj->radius *
	 	sqrt(1 - pow(obj->direction.z, 2));
	*aabb_min = vec3_sub(obj->position, extent);
	*aabb_max = vec3_add(obj->position, extent);
}

void	get_cone_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	extent[2];
	double	height[2];
	int		r_max;
	int		i;

	if (!aabb_min || !aabb_max)
		return ;
	height[0] = obj->props.height;
	height[1] = obj->props.depth;
	i = 0;
	while (i < 2)
	{
		r_max = fmax(height[0], height[1]) * obj->props.tan_angle;
		extent[i].x = height[i] * fabs(obj->direction.x) + r_max *
			sqrt(1 - pow(obj->direction.x, 2));
		extent[i].y = height[i] * fabs(obj->direction.y) + r_max *
			sqrt(1 - pow(obj->direction.y, 2));
		extent[i].z = height[i] * fabs(obj->direction.z) + r_max *
			sqrt(1 - pow(obj->direction.z, 2));
		i++;
	}
	*aabb_min = vec3_sub(obj->position, vec3_min(extent[0], extent[1]));
	*aabb_max = vec3_add(obj->position, vec3_max(extent[0], extent[1]));
}

void	get_box_aabb(t_list *elements, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	static const t_vec3	unit_vec = (t_vec3){1, 1, 1};
	t_object			*obj;

	*aabb_min = vec3_scale(unit_vec, -1e30);
	*aabb_max = vec3_scale(unit_vec, 1e30);
	while (elements)
	{
		obj = (t_object *)elements->content;
		if (obj->type == OBJ_CYLINDER)
			get_cylinder_aabb(obj, aabb_min, aabb_max);
		else if (obj->type == OBJ_CONE)
			get_cone_aabb(obj, aabb_min, aabb_max);
		else if (obj->type == OBJ_HYPERBOLOID)
			get_hyperboloid_aabb(obj, aabb_min, aabb_max);
		else if (obj->type == OBJ_PARABOLOID)
			get_paraboloid_aabb(obj, aabb_min, aabb_max);
		else if (obj->type == OBJ_TRIANGLE)
			get_triangle_aabb(obj, aabb_min, aabb_max);
		else
		{
			*aabb_min = vec3_min(*aabb_min,
				vec3_sub(obj->position, vec3_scale(unit_vec, obj->radius)));
			*aabb_max = vec3_max(*aabb_min,
				vec3_add(obj->position, vec3_scale(unit_vec, obj->radius)));
		}
	}
}
