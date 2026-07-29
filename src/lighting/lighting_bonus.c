/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:40:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/30 01:52:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "hit_bonus.h"
#include "normal.h"
#include <math.h>

double	get_object_hit_opacity(t_hit_record rec);

static double	shadow_attenuate(t_hit_record tmp, t_scene *scene,
	t_object *light)
{
	t_ray	s_ray;
	double	light_dist;
	double	s_factor;
	double	opacity;

	s_factor = 1.0;
	if (vec3_dot(vec3_sub(light->position, tmp.point), tmp.normal) < 0)
		return (0.0);
	light_dist = vec3_length(vec3_sub(light->position, tmp.point));
	s_ray.origin = tmp.point;
	s_ray.direction = vec3_normalize(vec3_sub(light->position, tmp.point));
	while (hit_scene(scene, &s_ray, light_dist, &tmp)
		&& tmp.t < light_dist)
	{
		if (scene->transparency == false)
			return (0.0);
		opacity = get_object_hit_opacity(tmp);
		s_factor *= (1 - opacity);
		if (s_factor < 1e-4)
			break ;
		light_dist -= tmp.t;
		s_ray.origin = vec3_add(tmp.point, vec3_scale(s_ray.direction, 1e-4));
	}
	return (s_factor);
}

static t_vec3	apply_ambient(t_vec3 color, t_object *ambient)
{
	t_vec3	ambient_color;
	t_vec3	ambient_linear;

	ambient_linear = vec3_pow(vec3_scale(ambient->color, 1.0 / 255.0), 2.2);
	ambient_color = vec3_scale(vec3_multiply(color, ambient_linear),
			ambient->props.intensity);
	return (ambient_color);
}

static void	apply_diffuse(t_hit_record *rec, t_object *light, t_vec3 *result)
{
	t_vec3	light_dir;
	t_vec3	diff_color;
	double	diff;

	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	diff = fmax(0.0, vec3_dot(rec->normal, light_dir));
	diff_color = vec3_scale(vec3_multiply(rec->color, light->color),
			diff * light->props.intensity);
	*result = vec3_add(*result, diff_color);
}

static void	apply_specular(t_hit_record *rec, t_object *light, t_vec3 *result,
	t_ray *ray)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	dot_ln;
	double	spec;

	if (rec->specular <= 0.0)
		return ;
	if (rec->shininess <= 0)
		rec->shininess = 128;
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	dot_ln = vec3_dot(light_dir, rec->normal);
	if (dot_ln <= 0)
		return ;
	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	reflect_dir = vec3_normalize(vec3_sub(
				vec3_scale(rec->normal, 2.0 * dot_ln), light_dir));
	spec = pow(fmax(0.0, vec3_dot(reflect_dir, view_dir)), rec->shininess);
	spec *= rec->specular * light->props.intensity;
	spec *= (rec->shininess + 8.0) / (8.0 * M_PI);
	*result = vec3_add(*result, vec3_scale(light->color, spec * rec->specular));
}

t_vec3	shade(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	t_hit_record	tmp;
	t_vec3			res;
	t_object		light;
	size_t			i;

	i = 0;
	tmp = *rec;
	tmp.color = vec3_pow(vec3_scale(rec->color, 1.0 / 255.0), 2.2);
	res = apply_ambient(tmp.color, scene->ambient);
	while (i < scene->lights.len)
	{
		light = *((t_object *)(scene->lights.array[i]));
		light.props.intensity *= shadow_attenuate(tmp, scene, &light);
		if (light.props.intensity >= 1e-4)
		{
			light.color = vec3_pow(vec3_scale(light.color, 1.0 / 255.0), 2.2);
			apply_diffuse(&tmp, &light, &res);
			if (scene->specular)
				apply_specular(&tmp, &light, &res, ray);
		}
		i++;
	}
	rec->color = vec3_scale(vec3_clamp(vec3_pow(res, 1.0 / 2.2), 0, 1), 255);
	ray_bounce(scene, rec, ray);
	return (rec->color);
}
