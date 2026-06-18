/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:48:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/18 16:36:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
#include "math.h"

static t_hit_fn	get_hit_fn(t_obj_type type)
{
	static t_hit_fn		hit_list[] = {
		[OBJ_PLANE] = hit_plane,
		[OBJ_SPHERE] = hit_sphere,
		[OBJ_CYLINDER] = hit_cylinder,
		[OBJ_CONE] = hit_cone,
		[OBJ_HYPERBOLOID] = hit_hyperboloid,
		[OBJ_PARABOLOID] = hit_paraboloid,
		[OBJ_TRIANGLE] = hit_triangle};

	if (type < OBJ_PLANE || type > OBJ_TRIANGLE)
		return (NULL);
	return (hit_list[type]);
}

int	hit_list(t_array obj, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	size_t			i;
	int				hit_curr;
	int				hit;
	t_hit_record	temp;
	t_hit_fn		hit_fn;

	hit = 0;
	i = 0;
	while (i < obj.len)
	{
		hit_curr = 0;
		hit_fn = get_hit_fn(((t_object *)obj.array[i])->type);
		if (hit_fn)
			hit_curr = hit_fn((t_object *)obj.array[i], ray, *closest, &temp);
		if (hit_curr && temp.t >= T_MIN && temp.t < *closest)
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		i++;
	}
	return (hit);
}

static int	hit_object_in_bvh(t_bvh *bvh, t_ray *ray, double *closest,
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

#if BVH_VIEW
static int	draw_box_bounds(t_bvh *bvh, t_ray *ray, double dist)
{
	static const double	epsilon = 5e-3;
	t_vec3				dist_to_min;
	t_vec3				dist_to_max;
	t_vec3				point;
	int					i;

	if (dist == 1e30)
		return (0);
	point = ray_at(*ray, dist);
	dist_to_min = vec3_sub(point, bvh->aabb.min);
	dist_to_max = vec3_sub(point, bvh->aabb.max);
	i = (fabs(dist_to_min.x) < epsilon * dist) ^ (fabs(dist_to_max.x) < epsilon * dist);
	i += (fabs(dist_to_min.y) < epsilon * dist) ^ (fabs(dist_to_max.y) < epsilon * dist);
	i += (fabs(dist_to_min.z) < epsilon * dist) ^ (fabs(dist_to_max.z) < epsilon * dist);
	return (i);
}
#endif

static int	hit_bvh(t_bvh *bvh, t_ray *ray, double *closest, t_hit_record *rec,
		int bvh_display_level)
{
	double			dist[2];
	int				depth;
	int				hit;
	t_bvh			*stack[2 * BVH_DEPTH];
	t_bvh			*node;
	t_bvh			*child[2];

	dist[0] = 1e30;
	dist[1] = 1e30;
	depth = 0;
	hit = 0;
	node = bvh;
	dist[0] = hit_bvh_box(node, ray, *closest);
	if (dist[0] == 1e30)
		return (0);
	while (1)
	{
		if (!node->left && !node->right)
		{
			if (hit_object_in_bvh(node, ray, closest, rec))
				hit = 1;
			if (depth == 0)
				break ;
			node = stack[--depth];
			continue ;
		}
#if BVH_VIEW
		int	draw_bounds = draw_box_bounds(node, ray, dist[0]);
		if (node->depth == bvh_display_level && draw_bounds)
		{
			rec->color = (t_vec3){{(node->depth << 6 | 0xf | draw_bounds << 4) & 0xff,
				((node->depth >> 2) << 6 | 0xf | draw_bounds << 4) & 0xff,
				64 * draw_bounds}};
			hit = 1;
		}
#endif
		dist[0] = 1e30;
		dist[1] = 1e30;
		child[0] = node->left;
		child[1] = node->right;
		if (child[0])
			dist[0] = hit_bvh_box(child[0], ray, *closest);
		if (child[1])
			dist[1] = hit_bvh_box(child[1], ray, *closest);
		if (dist[1] < dist[0])
		{
			ft_dswap(&dist[0], &dist[1]);
			ft_memswap(&child[0], &child[1], sizeof(t_bvh *));
		}
		if (dist[0] == 1e30)
		{
			if (!depth)
				break ;
			node = stack[--depth];
		}
		else
		{
			node = child[0];
			if (dist[1] != 1e30 && depth < 2 * BVH_DEPTH - 1)
				stack[depth++] = child[1];
		}
	}
	return (hit);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec, int bvh_display_level)
{
	int		hit[2];
	double	closest;

	hit[0] = 0;
	hit[1] = 0;
	closest = t_max;
	ray->inv_direction = (t_vec3){{1 / ray->direction.x, 1 / ray->direction.y,
		1 / ray->direction.z}};
	hit[0] = hit_list(scene->objects, ray, &closest, rec);
	hit[1] = hit_bvh(scene->bvh, ray, &closest, rec, bvh_display_level);
	return (hit[0] || hit[1]);
}
