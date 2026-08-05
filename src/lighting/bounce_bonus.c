/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounce_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 01:43:25 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/06 00:06:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit_bonus.h"
#include "object_bonus.h"
#include "miniRT_bonus.h"
#include <math.h>

double	get_object_hit_opacity(t_hit_record rec);

static t_vec3	get_refraction_vector(t_vec3 normal, t_vec3 incidence,
	double n1, double n2)
{
	double	n;
	double	cos_i;
	double	sin_t2;
	double	cos_t;

	cos_i = -vec3_dot(normal, incidence);
	if (cos_i < 0)
	{
		normal = vec3_scale(normal, -1);
		cos_i = -cos_i;
	}
	n = n1 / n2;
	sin_t2 = n * n * (1 - cos_i * cos_i);
	if (sin_t2 > 1.0)
		return (incidence);
	cos_t = sqrt(1.0 - sin_t2);
	return (vec3_normalize(vec3_add(
				vec3_scale(incidence, n),
				vec3_scale(normal, n * cos_i - cos_t))));
}

static t_vec3	apply_reflection(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	t_vec3	color;
	t_ray	reflect_ray;

	ft_bzero(&reflect_ray, sizeof(t_ray));
	reflect_ray.origin = vec3_add(rec->point, vec3_scale(rec->normal, 1e-4));
	reflect_ray.direction = vec3_normalize(
			vec3_sub(ray->direction, vec3_scale(
					rec->normal, 2 * vec3_dot(rec->normal, ray->direction))));
	reflect_ray.refraction = ray->refraction;
	color = rt_cast(scene, &reflect_ray, rec->object, rec->depth + 1);
	return (color);
}

static t_vec3	apply_refraction(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	t_vec3			color;
	t_ray			r_ray;

	ft_bzero(&r_ray, sizeof(t_ray));
	r_ray.origin = vec3_sub(rec->point, vec3_scale(rec->normal, 1e-4));
	r_ray.refraction = ray->refraction;
	if (rec->object->material->illum >= 3)
		r_ray.refraction = rec->object->material->density;
	r_ray.direction = ray->direction;
	if (rec->object->material->illum >= 3)
		r_ray.direction = get_refraction_vector(rec->normal, ray->direction,
				ray->refraction, rec->object->material->density);
	color = rt_cast(scene, &r_ray, rec->object, rec->depth + 1);
	return (color);
}

static double	fresnel_reflectance(t_hit_record *rec, t_ray *ray,
	double opacity, bool *can_refract)
{
	double	r0;
	double	cos_i;
	double	sin_t2;
	double	kr;

	*can_refract = false;
	if (ray->refraction <= 0.0 || rec->object->material->density <= 0.0)
		return (1.0);
	if (rec->object->material->illum <= 3 && opacity >= 1 - 1e-4)
		return (rec->object->material->reflectance
			* (rec->object->material->illum == 3));
	r0 = pow((ray->refraction - rec->object->material->density)
			/ (ray->refraction + rec->object->material->density), 2.0);
	cos_i = vec3_dot(rec->normal, ray->direction);
	if (cos_i < 0)
		cos_i = -cos_i;
	sin_t2 = pow(ray->refraction / rec->object->material->density, 2)
		* (1 - cos_i * cos_i);
	if (sin_t2 > 1.0)
		return (1.0);
	kr = r0 + (1 - r0) * pow(1 - cos_i, 5.0);
	*can_refract = true;
	return (kr);
}

void	ray_bounce(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	double	kr;
	t_vec3	reflect_color;
	t_vec3	refract_color;
	double	opacity;
	bool	can_refract;

	ft_bzero(&reflect_color, sizeof(t_vec3));
	ft_bzero(&refract_color, sizeof(t_vec3));
	if (!scene->transparency || rec->depth >= RAY_DEPTH)
		return ;
	opacity = get_object_hit_opacity(*rec);
	kr = fresnel_reflectance(rec, ray, opacity, &can_refract);
	if (kr > 1e-4 && rec->depth < RAY_DEPTH)
		reflect_color = apply_reflection(scene, rec, ray);
	if (can_refract && opacity < 1 - 1e-4 && kr < 1 - 1e-4
		&& rec->depth < RAY_DEPTH)
		refract_color = apply_refraction(scene, rec, ray);
	rec->color = vec3_add(
			vec3_scale(reflect_color, kr),
			vec3_scale(vec3_add(
					vec3_scale(rec->color, opacity),
					vec3_scale(refract_color, 1 - opacity)), 1.0 - kr));
}
