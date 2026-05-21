/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bvh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:38:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 20:53:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "assert.h"

static t_vec3	get_min_bounds(t_ray *ray, t_vec3 const *bounds,
		int const *sign)
{
	t_vec3	result;

	result.x = (bounds[sign[0]].x - ray->origin.x) * ray->inv_direction.x;
	result.y = (bounds[sign[1]].y - ray->origin.y) * ray->inv_direction.y;
	result.z = (bounds[sign[2]].z - ray->origin.z) * ray->inv_direction.z;
	return (result);
}

static t_vec3	get_max_bounds(t_ray *ray, t_vec3 const *bounds,
		int const *sign)
{
	t_vec3	result;

	result.x = (bounds[1 - sign[0]].x - ray->origin.x) * ray->inv_direction.x;
	result.y = (bounds[1 - sign[1]].y - ray->origin.y) * ray->inv_direction.y;
	result.z = (bounds[1 - sign[2]].z - ray->origin.z) * ray->inv_direction.z;
	return (result);
}

int	hit_bvh_box(t_bvh *bvh, t_ray *ray, double *dist, t_vec3 *point)
{
	const int		sign[3] = {ray->inv_direction.x < 0,
		ray->inv_direction.y < 0, ray->inv_direction.z < 0};
	t_vec3			v_min;
	t_vec3			v_max;
	const t_vec3	bounds[2] = {bvh->aabb_min, bvh->aabb_max};

	v_min = get_min_bounds(ray, bounds, sign);
	v_max = get_max_bounds(ray, bounds, sign);
	if ((v_min.x > v_max.y) || (v_min.y > v_max.x))
		return (0);
	if (v_min.y > v_min.x)
		v_min.x = v_min.y;
	if (v_max.y < v_max.x)
		v_max.x = v_max.y;
	if ((v_min.x > v_max.z) || (v_min.z > v_max.x))
		return (0);
	if (v_min.z > v_min.x)
		v_min.x = v_min.z;
	if (v_max.z < v_max.x)
		v_max.x = v_max.z;
	if (v_min.x < *dist && v_max.x > T_MIN)
	{
		*point = ray_at(*ray, v_min.x);
		*dist = v_min.x;
		return (1);
	}
	return (0);
}
