/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bvh_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 21:32:15 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 23:55:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"

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

int	hit_object_in_bvh(t_bvh *bvh, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	int		hit;
	t_array	slice;

	slice = (t_array){
		.array = &bvh->objects.array[bvh->first_index],
		.len = bvh->nb_elements,
		.size = bvh->nb_elements
	};
	hit = hit_list(slice, ray, closest, rec);
	return (hit);
}

int	draw_box_bounds(t_bvh_hit_ctx *ctx, t_bvh_state state)
{
	static const double	epsilon = 5e-3;
	t_vec3				dist_to_min;
	t_vec3				dist_to_max;
	int					i;

	if (state.dist[0] == 1e30)
		return (0);
	dist_to_min = vec3_sub(ray_at(*ctx->ray, state.dist[0]),
			state.node->aabb.min);
	dist_to_max = vec3_sub(ray_at(*ctx->ray, state.dist[0]),
			state.node->aabb.max);
	i = (fabs(dist_to_min.x) < epsilon * state.dist[0])
		^ (fabs(dist_to_max.x) < epsilon * state.dist[0]);
	i += (fabs(dist_to_min.y) < epsilon * state.dist[0])
		^ (fabs(dist_to_max.y) < epsilon * state.dist[0]);
	i += (fabs(dist_to_min.z) < epsilon * state.dist[0])
		^ (fabs(dist_to_max.z) < epsilon * state.dist[0]);
	if (state.node->depth != ctx->scene->bvh_display_level || !i)
		return (0);
	ctx->rec->color = (t_vec3){{(state.node->depth << 6 | 0xf | i << 4) & 0xff,
		((state.node->depth >> 2) << 6 | 0xf | i << 4) & 0xff, i << 6}};
	return (1);
}
