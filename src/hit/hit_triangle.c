/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_triangle.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/02 22:58:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 22:24:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	update_hit_record(t_hit_record *rec, double t, t_ray *ray,
		t_object *obj)
{
	rec->t = t;
	rec->point = ray_at(*ray, t);
	rec->normal = face_normal(ray, obj->direction);
	rec->object = obj;
	rec->specular = obj->specular;
	rec->shininess = obj->shininess;
}

static void	apply_checker(t_hit_record *rec)
{
	int		x;
	int		z;

	x = (int)floor(rec->point.x);
	z = (int)floor(rec->point.z);
	if ((x + z) % 2 == 0)
		rec->color = (t_vec3){255, 255, 255};
	else
		rec->color = (t_vec3){30, 30, 30};
}

static int	intersect_triangle_area(t_object *obj, t_vec3 point)
{
	t_vec3	obj_hit;
	double	dot[2][3];
	double	denom;
	double	u;
	double	v;

	obj_hit = vec3_sub(point, obj->position);
	dot[0][0] = vec3_dot(obj->props.c, obj->props.c);
	dot[0][1] = vec3_dot(obj->props.c, obj->props.b);
	dot[0][2] = vec3_dot(obj->props.c, obj_hit);
	dot[1][1] = vec3_dot(obj->props.b, obj->props.b);
	dot[1][2] = vec3_dot(obj->props.b, obj_hit);
	denom = dot[0][0] * dot[1][1] - pow(dot[0][1], 2);
	if (fabs(denom) < 1e-10)
		return (0);
	u = (dot[1][1] * dot[0][2] - dot[0][1] * dot[1][2]) / denom;
	v = (dot[0][0] * dot[1][2] - dot[0][1] * dot[0][2]) / denom;
	return ((u >= 0) && (v >= 0) && ((u + v) <= 1));
}

int	hit_triangle(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	d;
	double	t;
	t_vec3	point;

	d = vec3_dot(ray->direction, obj->direction);
	if (fabs(d) < 1e-6)
		return (0);
	t = vec3_dot(vec3_sub(obj->position, ray->origin), obj->direction) / d;
	if (t < T_MIN || t > t_max)
		return (0);
	point = ray_at(*ray, t);
	if (!intersect_triangle_area(obj, point))
		return (0);
	update_hit_record(rec, t, ray, obj);
	if (obj->checker)
		apply_checker(rec);
	else
		rec->color = obj->color;
	return (1);
}
