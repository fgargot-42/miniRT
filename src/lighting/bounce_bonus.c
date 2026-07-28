/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bounce_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 01:43:25 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/28 19:48:20 by fgargot          ###   ########.fr       */
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

static void	apply_reflection(t_hit_record *rec)
{
	(void)rec;
	return ;
}

static void	apply_refraction(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	t_vec3			color;
	t_ray			r_ray;
	double			opacity;

	opacity = get_object_hit_opacity(*rec);
	if (opacity >= 1.0 - 1e-4 || opacity < 0.0)
		return ;
	ft_bzero(&r_ray, sizeof(t_ray));
	r_ray.origin = rec->point;
	r_ray.direction = get_refraction_vector(rec->normal, ray->direction,
			ray->refraction, rec->object->material->density);
	r_ray.refraction = rec->object->material->density;
	rec->depth++;
	color = rt_cast(scene, &r_ray, rec->object, rec->depth);
	color = vec3_scale(color, 1 - opacity);
	color = vec3_add(color, vec3_scale(rec->color, opacity));
	rec->color = color;
}

void	ray_bounce(t_scene *scene, t_hit_record *rec, t_ray *ray)
{
	if (rec->depth == RAY_DEPTH)
		return ;
	apply_reflection(rec);
	apply_refraction(scene, rec, ray);
}
