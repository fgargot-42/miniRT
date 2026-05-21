/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:27:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 22:40:03 by fgargot          ###   ########.fr       */
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
	extent.x = obj->props.height / 2.0 * fabs(obj->direction.x) + obj->radius
		* sqrt(1 - pow(obj->direction.x, 2));
	extent.y = obj->props.height / 2.0 * fabs(obj->direction.y) + obj->radius
		* sqrt(1 - pow(obj->direction.y, 2));
	extent.z = obj->props.height / 2.0 * fabs(obj->direction.z) + obj->radius
		* sqrt(1 - pow(obj->direction.z, 2));
	*aabb_min = vec3_sub(obj->position, extent);
	*aabb_max = vec3_add(obj->position, extent);
}

void	get_cone_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	extent[2];
	double	height[2];
	double	r_max[2];
	int		i;

	if (!aabb_min || !aabb_max)
		return ;
	height[0] = obj->props.height;
	height[1] = obj->props.depth;
	i = 0;
	while (i < 2)
	{
		r_max[i] = height[i] * obj->props.tan_angle;
		extent[i].x = height[i] * fabs(obj->direction.x) + r_max[i]
			* sqrt(1 - pow(obj->direction.x, 2));
		extent[i].y = height[i] * fabs(obj->direction.y) + r_max[i]
			* sqrt(1 - pow(obj->direction.y, 2));
		extent[i].z = height[i] * fabs(obj->direction.z) + r_max[i]
			* sqrt(1 - pow(obj->direction.z, 2));
		i++;
	}
	extent[0] = vec3_max(extent[0], extent[1]);
	*aabb_min = vec3_sub(obj->position, extent[0]);
	*aabb_max = vec3_add(obj->position, extent[0]);
}
