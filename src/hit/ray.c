/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:28:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 21:40:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	ray_at(t_ray ray, double t)
{
	return (vec3_add(ray.origin, vec3_scale(ray.direction, t)));
}

t_ray	get_object_relative_ray(t_ray ray, t_object *obj)
{
	t_ray	result;

	result.origin = vec3_sub(ray.origin, obj->position);
	result.direction = ray.direction;
	if (fabs(obj->direction.z - 1) > 1e-3)
	{
		result.origin = vec_apply_rotation_z(result.origin,
				obj->props.transform_axis);
		result.direction = vec_apply_rotation_z(result.direction,
				obj->props.transform_axis);
	}
	return (result);
}
