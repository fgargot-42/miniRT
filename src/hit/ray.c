/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 19:28:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 01:13:29 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"

t_vec3	ray_at(t_ray ray, double t)
{
	t -= fmax(1e-8, 1e-8 * t);
	return (vec3_add(ray.origin, vec3_scale(ray.direction, t)));
}

t_ray	get_object_relative_ray(t_ray ray, t_object *obj)
{
	t_ray	result;

	result.origin = vec3_sub(ray.origin, obj->position);
	result.direction = ray.direction;
	result.origin = vec_apply_matrix(result.origin,
			obj->props.transform_axis);
	result.direction = vec_apply_matrix(result.direction,
			obj->props.transform_axis);
	return (result);
}
