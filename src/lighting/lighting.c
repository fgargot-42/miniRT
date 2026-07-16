/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:40:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 00:55:53 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <math.h>

static int	in_shadow(t_hit_record tmp, t_scene *scene, t_object *light)
{
	t_ray	shadow_ray;
	t_vec3	to_light;
	double	light_dist;
	bool	is_hit;

	to_light = vec3_sub(light->position, tmp.point);
	if (vec3_dot(to_light, tmp.normal) < 0)
		return (1);
	light_dist = vec3_length(to_light);
	shadow_ray.origin = tmp.point;
	shadow_ray.direction = vec3_normalize(to_light);
	is_hit = hit_scene(scene, &shadow_ray, light_dist, &tmp);
	return (is_hit);
}

t_vec3	apply_ambient(t_vec3 color, t_object *ambient)
{
	t_vec3	ambient_color;
	t_vec3	ambient_linear;
	
	ambient_linear = vec3_pow(vec3_scale(ambient->color, 1.0 / 255.0), 2.2);
	ambient_color = vec3_scale(vec3_multiply(color, ambient_linear),
			ambient->props.intensity);
	return (ambient_color);
}

t_vec3	apply_diffuse(t_hit_record *rec, t_object *light)
{
	t_vec3	light_dir;
	t_vec3	diff_color;
	double	diff;

	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	diff = smoothstep(-0.05, 0.15, vec3_dot(rec->normal, light_dir));
	diff_color = vec3_scale(rec->color, diff * light->props.intensity);
	return (diff_color);
}

t_vec3	shade(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	t_hit_record	tmp;
	t_vec3			linear_color;
	t_vec3			result;

	tmp = *rec;
	tmp.normal = face_normal(ray, rec->normal);
	linear_color = vec3_pow(vec3_scale(rec->color, 1.0 / 255.0), 2.2);
	tmp.color = linear_color;
	result = apply_ambient(linear_color, scene->ambient);
	if (!in_shadow(tmp, scene, scene->light))
		result = vec3_add(result, apply_diffuse(&tmp, scene->light));
	rec->color = vec3_scale(vec3_clamp(vec3_pow(result, 1.0 / 2.2), 0.0, 1.0), 255.0);
	return (rec->color);
}
