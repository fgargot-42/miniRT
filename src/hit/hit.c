/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:48:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 21:59:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
#include "assert.h"

static t_hit_fn	*get_hit_fn(t_obj_type type)
{
	int					i;
	static const int	hit_list_size = 7;
	static t_hit_fn		hit_list[] = {
	{OBJ_PLANE, hit_plane}, {OBJ_SPHERE, hit_sphere},
	{OBJ_CYLINDER, hit_cylinder}, {OBJ_CONE, hit_cone},
	{OBJ_HYPERBOLOID, hit_hyperboloid}, {OBJ_PARABOLOID, hit_paraboloid},
	{OBJ_TRIANGLE, hit_triangle}};

	i = 0;
	while (i < hit_list_size)
	{
		if (type == hit_list[i].type)
			return (&hit_list[i]);
		i++;
	}
	return (NULL);
}

int	hit_list(t_list *obj, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	t_hit_record	temp;
	int				hit_current;
	t_object		*current;
	t_hit_fn		*hit_func;
	int				hit;

	hit = 0;
	while (obj)
	{
		current = (t_object *)obj->content;
		hit_current = 0;
		hit_func = get_hit_fn(current->type);
		if (current->type == OBJ_PLANE && hit_func)
			hit_current = hit_func->hit_fn(current, ray, *closest, &temp);
		if (hit_current && temp.t >= T_MIN && temp.t < *closest)
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		obj = obj->next;
	}
	return (hit);
}

static int	hit_object_in_bvh(t_bvh *bvh, t_ray *ray, double *closest,
	t_hit_record *rec)
{
	int				i;
	t_hit_record	temp;
	t_hit_fn		*hit_func;
	int				hit;
	int				hit_current;

	i = bvh->first_index;
	hit = 0;
	while (i < bvh->first_index + bvh->nb_elements)
	{
		hit_func = get_hit_fn(bvh->objects[i]->type);
		if (hit_func)
			hit_current = hit_func->hit_fn(bvh->objects[i], ray,
					*closest, &temp);
		if (hit_current && temp.t >= T_MIN && temp.t < *closest)
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		i++;
	}
	return (hit);
}
#if BVH_VIEW
static int	draw_box_bounds(t_bvh *bvh, t_vec3 point, double dist)
{
	static const double	epsilon = 5e-3;
	t_vec3				dist_to_min;
	t_vec3				dist_to_max;
	int					i;

	dist_to_min = vec3_sub(point, bvh->aabb_min);
	dist_to_max = vec3_sub(point, bvh->aabb_max);
	i = (fabs(dist_to_min.x) < epsilon * dist) ^ (fabs(dist_to_max.x) < epsilon * dist);
	i += (fabs(dist_to_min.y) < epsilon * dist) ^ (fabs(dist_to_max.y) < epsilon * dist);
	i += (fabs(dist_to_min.z) < epsilon * dist) ^ (fabs(dist_to_max.z) < epsilon * dist);
	return (i >= 2);
}
#endif

static int	hit_bvh(t_bvh *bvh, t_ray *ray, double *closest, t_hit_record *rec,
	int	display_bvh_depth)
{
	int				hit[3];
	double			dist;
	t_vec3			point;

	hit[0] = 0;
	hit[1] = 0;
	hit[2] = 0;
	dist = *closest;
	point = (t_vec3){0, 0, 0};
	if (!hit_bvh_box(bvh, ray, &dist, &point))
		return (0);
#if BVH_VIEW
	if (bvh->depth == display_bvh_depth && draw_box_bounds(bvh, point, dist))
	{
		rec->color = (t_vec3){(bvh->depth << 6) & 0xff, 
		((bvh->depth >> 2) << 6) & 0xff, 64};
		hit[2] = 1;
	}
#endif
	if (!bvh->left && !bvh->right)
		return (hit_object_in_bvh(bvh, ray, closest, rec) || hit[2]);
	if (bvh->left)
		hit[0] = hit_bvh(bvh->left, ray, closest, rec, display_bvh_depth);
	if (bvh->right)
		hit[1] = hit_bvh(bvh->right, ray, closest, rec, display_bvh_depth);
	return (hit[0] || hit[1] || hit[2]);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec,
	int display_bvh_depth)
{
	int		hit;
	double	closest;

	hit = 0;
	closest = t_max;
	ray->inv_direction = (t_vec3){1 / ray->direction.x, 1 / ray->direction.y,
		1 / ray->direction.z};
	hit = hit_list(scene->objects, ray, &closest, rec);
	hit = hit_bvh(scene->bvh, ray, &closest, rec, display_bvh_depth) | hit;
	return (hit);
}
