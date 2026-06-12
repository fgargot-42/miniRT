/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bvh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:38:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/12 17:42:12 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

double	hit_bvh_box(t_bvh *bvh, t_ray *ray, double dist)
{
	t_aabb	aabb;
	double	tmin;
	double	tmax;

	if (!bvh)
		return (1e30);
	aabb.min.x = (bvh->aabb.min.x - ray->origin.x) * ray->inv_direction.x;
	aabb.max.x = (bvh->aabb.max.x - ray->origin.x) * ray->inv_direction.x;
	tmin = fmin(aabb.min.x, aabb.max.x);
	tmax = fmax(aabb.min.x, aabb.max.x);
	aabb.min.y = (bvh->aabb.min.y - ray->origin.y) * ray->inv_direction.y;
	aabb.max.y = (bvh->aabb.max.y - ray->origin.y) * ray->inv_direction.y;
	tmin = fmax(tmin, fmin(aabb.min.y, aabb.max.y));
	tmax = fmin(tmax, fmax(aabb.min.y, aabb.max.y));
	aabb.min.z = (bvh->aabb.min.z - ray->origin.z) * ray->inv_direction.z;
	aabb.max.z = (bvh->aabb.max.z - ray->origin.z) * ray->inv_direction.z;
	tmin = fmax(tmin, fmin(aabb.min.z, aabb.max.z));
	tmax = fmin(tmax, fmax(aabb.min.z, aabb.max.z));
	if (tmax >= tmin && tmin < dist && tmax > T_MIN)
		return (fmax(tmin, T_MIN));
	return (1e30);
}
