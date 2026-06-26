/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 17:40:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/29 23:01:06 by fgargot          ###   ########.fr       */
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
	return ((t_vec3){{
			color.x * ambient->color.x * ambient->props.intensity / 255.0,
			color.y * ambient->color.y * ambient->props.intensity / 255.0,
			color.z * ambient->color.z * ambient->props.intensity / 255.0,
		}});
}

t_vec3	apply_diffuse(t_hit_record *rec, t_object *light)
{
	t_vec3	light_dir;
	double	diff;

	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	diff = fmax(0.0, vec3_dot(rec->normal, light_dir));
	return ((t_vec3){{
			rec->color.x * light->color.x / 255.0
			* diff * light->props.intensity,
			rec->color.y * light->color.y / 255.0
			* diff * light->props.intensity,
			rec->color.z * light->color.z / 255.0
			* diff * light->props.intensity,
		}});
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
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	dot_ln = vec3_dot(light_dir, rec->normal);
	reflect_dir = vec3_sub(vec3_scale(rec->normal, 2.0 * dot_ln), light_dir);
	spec = pow(fmax(0.0, vec3_dot(reflect_dir, view_dir)), rec->shininess);
	spec *= rec->specular * light->props.intensity;
	return ((t_vec3){{
			light->color.x * spec,
			light->color.y * spec,
			light->color.z * spec,
		}});
}

t_vec3	apply_normal_map(t_hit_record *rec)
{
	t_vec3	new_normal;
	t_vec3	normal;
	t_vec2	delta_uv[2];
	t_vec3	edge[2];
	t_vec3	tans[2];
	t_mat3	TBN;
	double	f;
	
	normal = triangle_uv_to_color(rec->object, rec->point);
	normal = vec3_sub(vec3_scale(normal, 1.0 / 127.5), (t_vec3){{1.0, 1.0, 1.0}});
	normal = vec3_normalize(normal);
	// 
	delta_uv[0] = vec2_sub(rec->object->uv.tex_b, rec->object->uv.tex_a);
	delta_uv[1] = vec2_sub(rec->object->uv.tex_c, rec->object->uv.tex_a);
	edge[0] = vec3_sub(rec->object->props.b, rec->object->props.a);
	edge[1] = vec3_sub(rec->object->props.c, rec->object->props.a);
	f = 1.0 / (delta_uv[0].x *delta_uv[1].y - delta_uv[0].y * delta_uv[1].x);
	tans[0] = vec3_scale(vec3_sub(vec3_scale(edge[0], delta_uv[1].y),
				vec3_scale(edge[1], delta_uv[0].y)), f);
	tans[1] = vec3_scale(vec3_sub(vec3_scale(edge[1], delta_uv[0].x),
				vec3_scale(edge[0], delta_uv[1].x)), f);
	tans[0] = vec3_normalize(vec3_sub(tans[0],
				vec3_scale(rec->normal, vec3_dot(rec->normal, tans[0]))));
	TBN = init_mat3(tans[0], tans[1], rec->normal);
	TBN = transpose_mat3(TBN);
	new_normal = vec3_normalize(mat3_vec_mult(TBN, normal));
	return (new_normal);
}

t_vec3	shade(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	bool			is_shadow;
	t_hit_record	tmp;
	t_vec3			result;
	size_t			i;

	i = 0;
	tmp = *rec;
	if (rec->object->material && rec->object->material->normal_tex)
		tmp.normal = apply_normal_map(rec);
	rec->color = apply_ambient(rec->color, scene->ambient);
	result = rec->color;
	is_shadow = 1;
	while (i < scene->lights.len)
	{
		if (!in_shadow(tmp, scene, scene->lights.array[i]))
		{
			is_shadow = 0;
			result = vec3_add(result, apply_diffuse(&tmp,
						scene->lights.array[i]));
			result = vec3_add(result, apply_specular(&tmp,
						scene->lights.array[i], ray));
		}
		i++;
	}
	if (!is_shadow)
		rec->color = vec3_clamp(result, 0.0, 255.0);
	return (rec->color);
}
