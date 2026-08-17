/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:05:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 20:33:12 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "hit_bonus.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, double t, t_ray *ray,
	t_object *obj)
{
	rec->t = t;
	rec->point = ray_at(*ray, t);
	rec->normal = obj->direction;
	rec->color = obj->color;
	rec->object = obj;
	rec->specular = obj->material->specular;
	rec->shininess = obj->material->shininess;
}

int	hit_plane(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	d;
	double	t;
	t_vec3	local;

	d = vec3_dot(ray->direction, obj->direction);
	if (fabs(d) < 1e-8)
		return (0);
	t = vec3_dot(vec3_sub(obj->position, ray->origin), obj->direction) / d;
	if (t < T_MIN || t > t_max)
		return (0);
	update_hit_record(rec, t, ray, obj);
	local = vec_apply_matrix(vec3_sub(rec->point, obj->position),
			obj->props.transform_axis);
	apply_checker(rec, obj, local);
	return (1);
}
