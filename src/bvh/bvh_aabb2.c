/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 22:07:32 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/18 22:16:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

void	get_hyperboloid_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	extent[2];
	double	height[2];
	double	r_max;
	int		i;

	if (!aabb_min || !aabb_max)
		return ;
	height[0] = obj->props.height;
	height[1] = obj->props.depth;
	i = 0;
	while (i < 2)
	{
		r_max = obj->radius * sqrt(1 + pow(height[i]
					/ (obj->radius * obj->props.tan_angle), 2));
		extent[i].x = height[i] * fabs(obj->direction.x) + r_max
			* sqrt(1 - pow(obj->direction.x, 2));
		extent[i].y = height[i] * fabs(obj->direction.y) + r_max
			* sqrt(1 - pow(obj->direction.y, 2));
		extent[i].z = height[i] * fabs(obj->direction.z) + r_max
			* sqrt(1 - pow(obj->direction.z, 2));
		i++;
	}
	*aabb_min = vec3_sub(obj->position, vec3_min(extent[0], extent[1]));
	*aabb_max = vec3_add(obj->position, vec3_max(extent[0], extent[1]));
}

void	get_paraboloid_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	t_vec3	cap_center;
	t_vec3	bulge;
	double	r_max;

	if (!aabb_min || !aabb_max)
		return ;
	cap_center = vec3_add(obj->position,
			vec3_scale(obj->direction, obj->props.height));
	r_max = obj->props.tan_angle * sqrt(obj->props.height);
	bulge.x = sqrt(1 - pow(obj->direction.x, 2));
	bulge.y = sqrt(1 - pow(obj->direction.y, 2));
	bulge.z = sqrt(1 - pow(obj->direction.z, 2));
	bulge = vec3_scale(bulge, r_max);
	*aabb_min = vec3_min(vec3_sub(cap_center, bulge), obj->position);
	*aabb_max = vec3_max(vec3_add(cap_center, bulge), obj->position);
}

void	get_triangle_aabb(t_object *obj, t_vec3 *aabb_min, t_vec3 *aabb_max)
{
	*aabb_min = vec3_min(vec3_min(obj->props.a, obj->props.b), obj->props.c);
	*aabb_max = vec3_max(vec3_max(obj->props.a, obj->props.b), obj->props.c);
}
