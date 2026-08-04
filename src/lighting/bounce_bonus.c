/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounce_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 01:43:25 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/04 02:44:52 by fgargot          ###   ########.fr       */
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

	if (n2 == 0)
		return (incidence);
	n = n1 / n2;
	cos_i = -vec3_dot(normal, incidence);
	if (cos_i < 0)
	{
		normal = vec3_scale(normal, -1);
		cos_i = -cos_i;
	}
	sin_t2 = n * n * (1 - cos_i * cos_i);
	if (sin_t2 > 1.0)
		return (incidence);
	cos_t = sqrt(1.0 - sin_t2);
	return (vec3_normalize(vec3_add(
				vec3_scale(incidence, n),
				vec3_scale(normal, n * cos_i - cos_t))));
}

static double	get_object_hit_reflection(t_hit_record rec)
{
	(void)rec;
	return (0.6);
}

static void	apply_reflection(t_scene *scene, t_hit_record *rec, t_ray *ray,
	double factor)
{
	t_vec3	color;
	t_ray	reflect_ray;
	double	r_factor;
	
	r_factor = get_object_hit_reflection(*rec);
	if (r_factor > 1.0 || r_factor <= 0.0)
		return ;
	ft_bzero(&reflect_ray, sizeof(t_ray));
	reflect_ray.origin = vec3_add(rec->point, vec3_scale(rec->normal, 1e-4));
	reflect_ray.direction = vec3_normalize(vec3_sub(ray->direction,
		vec3_scale(rec->normal, 2 * vec3_dot(rec->normal, ray->direction))));
	reflect_ray.refraction = ray->refraction;
	rec->depth++;
	color = rt_cast(scene, &reflect_ray, rec->object, rec->depth);
	color = vec3_scale(color, 1 - (r_factor * factor));
	color = vec3_add(color, vec3_scale(rec->color, r_factor * factor));
	rec->color = color;
	return ;
}

static void	apply_refraction(t_scene *scene, t_hit_record *rec, t_ray *ray,
	double factor)
{
	t_vec3			color;
	t_ray			r_ray;
	double			opacity;

	opacity = get_object_hit_opacity(*rec);
	if (opacity >= 1.0 - 1e-4 || opacity < 0.0)
		return ;
	ft_bzero(&r_ray, sizeof(t_ray));
	r_ray.origin = vec3_add(rec->point, vec3_scale(rec->normal, -1e-4));
	r_ray.direction = ray->direction;
	if (rec->object->material->illum >= 4)
		r_ray.direction = get_refraction_vector(rec->normal, ray->direction,
				ray->refraction, rec->object->material->density);
	r_ray.refraction = 1.0;
	if (rec->object->material->illum >= 4)
		r_ray.refraction = rec->object->material->density;
	rec->depth++;
	color = rt_cast(scene, &r_ray, rec->object, rec->depth);
	color = vec3_scale(color, 1 - (opacity * factor));
	color = vec3_add(color, vec3_scale(rec->color, opacity * factor));
	rec->color = color;
}

void	ray_bounce(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	//double	n1;
	//double	n2;
	//double	r0;
	//double	cos_i;
	//double	fresnel;

	//fresnel = 0.0;
	//if (rec->object->material->illum >= 3)
	//{
	//	n1 = ray->refraction;
	//	n2 = rec->object->material->density;
	//	r0 = pow((n1 - n2) / (n1 + n2), 2.0);
	//	cos_i = vec3_dot(rec->normal, ray->direction);
	//	if (cos_i >= 0.0)
	//		fresnel = r0 + (1 - r0) * pow(1 - cos_i, 5.0);
	//}
	if (!scene->transparency)
		return ;
	if (rec->depth >= RAY_DEPTH)
		return ;
	apply_reflection(scene, rec, ray, 1);
	apply_refraction(scene, rec, ray, 1);
}
