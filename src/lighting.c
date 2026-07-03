/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:40:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/03 01:40:46 by fgargot          ###   ########.fr       */
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
	diff = fmax(0.0, vec3_dot(rec->normal, light_dir));
	diff_color = vec3_scale(vec3_multiply(rec->color, light->color),
			diff * light->props.intensity);
	return (diff_color);
}

t_vec3	apply_specular(t_hit_record *rec, t_object *light, t_ray *ray)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	dot_ln;
	double	spec;

	if (rec->specular <= 0.0)
		return ((t_vec3){{0, 0, 0}});
	if (rec->shininess <= 0)
		rec->shininess = 32;
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	dot_ln = vec3_dot(light_dir, rec->normal);
	if (dot_ln <= 0)
		return ((t_vec3){{0, 0, 0}});
	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	reflect_dir = vec3_normalize(vec3_sub(vec3_scale(rec->normal, 2.0 * dot_ln), light_dir));
	spec = pow(fmax(0.0, vec3_dot(reflect_dir, view_dir)), rec->shininess);
	spec *= rec->specular * light->props.intensity;
	return (vec3_scale(light->color, spec));
}

t_vec3	shade(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	t_hit_record	tmp;
	t_vec3			linear_color;
	t_vec3			result;
	t_object		tmp_light;
	size_t			i;

	i = 0;
	tmp = *rec;
	//if (rec->object->material && rec->object->material->normal_tex)
	//	tmp.normal = apply_normal_map(rec);
	linear_color = vec3_pow(vec3_scale(rec->color, 1.0 / 255.0), 2.2);
	tmp.color = linear_color;
	result = apply_ambient(linear_color, scene->ambient);
	while (i < scene->lights.len)
	{
		tmp_light = *((t_object *)(scene->lights.array[i]));
		if (!in_shadow(tmp, scene, &tmp_light))
		{
			tmp_light.color = vec3_pow(vec3_scale(tmp_light.color, 1.0 / 255.0), 2.2);
			result = vec3_add(result, apply_diffuse(&tmp, &tmp_light));
			result = vec3_add(result, apply_specular(&tmp, &tmp_light, ray));
		}
		i++;
	}
	rec->color = vec3_scale(vec3_clamp(vec3_pow(result, 1.0 / 2.2), 0.0, 1.0), 255.0);
	return (rec->color);
}
