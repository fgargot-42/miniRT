/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 21:48:39 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/14 00:40:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

t_vec3	face_normal(t_ray *ray, t_vec3 inverted)
{
	if (vec3_dot(ray->direction, inverted) > 0)
		return (vec3_scale(inverted, -1.0));
	return (inverted);
}

static t_hit_fn	*get_hit_fn(t_obj_type type)
{
	int					i;
	static const int	hit_list_size = 8;
	static t_hit_fn		hit_list[] = {
	{OBJ_PLANE, hit_plane}, {OBJ_SPHERE, hit_sphere},
	{OBJ_CYLINDER, hit_cylinder}, {OBJ_CONE, hit_cone},
	{OBJ_HYPERBOLOID, hit_hyperboloid}, {OBJ_PARABOLOID, hit_paraboloid},
	{OBJ_TRIANGLE, hit_triangle}, {OBJ_BOX, hit_box}};

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

static int	hit_bvh(t_bvh *bvh, t_ray *ray, double *closest, t_hit_record *rec)
{
	int				i;
	t_hit_record	temp;
	t_hit_fn		*hit_func;
	int				hit;
	int				hit_current;

	hit = 0;
	if (!hit_bvh_box(bvh, ray, *closest))
		return (0);
	if (!bvh->left && bvh->right)
	{
		i = bvh->first_index;
		while (i < bvh->first_index + bvh->nb_elements)
		{
			hit_func = get_hit_fn(bvh->objects[i]->type);
			if (hit_func)
				hit_current = hit_func->hit_fn(bvh->objects[i], ray, *closest, &temp);
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
	if (bvh->left && hit_bvh_box(bvh->left, ray, *closest))
		hit = hit_bvh(bvh->left, ray, closest, rec);
	if (bvh->right && hit_bvh_box(bvh->right, ray, *closest))
		hit = hit_bvh(bvh->right, ray, closest, rec) | hit;
	return (hit);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec)
{
	int		hit;
	double	closest;

	hit = 0;
	closest = t_max;
	ray->inv_direction = (t_vec3){1 / ray->direction.x, 1 / ray->direction.y,
		1 / ray->direction.z};
	hit = hit_bvh(scene->bvh, ray, &closest, rec);
	hit = hit_list(scene->objects, ray, &closest, rec) | hit;
	return (hit);
}
