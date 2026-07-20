/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb_object.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 18:27:23 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:06:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Fuctions to get aabb (axis-aligned bounding box) for each object

#include "miniRT.h"
#include "bvh.h"
#include "veclib.h"

void	get_sphere_aabb(t_object *obj, t_aabb *aabb)
{
	t_vec3	extent;

	if (!aabb)
		return ;
	extent = vec3_scale((t_vec3){{1, 1, 1}}, obj->radius);
	aabb->min = vec3_sub(obj->position, extent);
	aabb->max = vec3_add(obj->position, extent);
}

void	get_cylinder_aabb(t_object *obj, t_aabb *aabb)
{
	t_vec3	extent;

	if (!aabb)
		return ;
	extent.x = obj->props.height / 2.0 * fabs(obj->direction.x) + obj->radius
		* sqrt(1 - pow(obj->direction.x, 2));
	extent.y = obj->props.height / 2.0 * fabs(obj->direction.y) + obj->radius
		* sqrt(1 - pow(obj->direction.y, 2));
	extent.z = obj->props.height / 2.0 * fabs(obj->direction.z) + obj->radius
		* sqrt(1 - pow(obj->direction.z, 2));
	aabb->min = vec3_sub(obj->position, extent);
	aabb->max = vec3_add(obj->position, extent);
}
