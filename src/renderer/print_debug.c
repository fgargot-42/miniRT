/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:32:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/28 22:48:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	in_shadow(t_hit_record *rec, t_hit_record tmp, t_scene *scene,
	t_light *light)
{
	t_ray			shadow_ray;
	t_vec3			to_light;
	double			light_dist;
	bool			is_hit;

	to_light = vec3_sub(light->position, tmp.point);
	light_dist = vec3_length(to_light);
	shadow_ray.origin = tmp.point;
	shadow_ray.direction = vec3_normalize(to_light);
	printf("Check obstacle to light: x=%.3f y=%.3f z=%.3f (d=%.3f):\n", light->position.x,
		light->position.y, light->position.z, light_dist);
	printf("\tRay direction: x=%.3f y=%.3f z=%.3f\n", shadow_ray.direction.x, shadow_ray.direction.y,
		shadow_ray.direction.z);
	is_hit = hit_scene(scene, &shadow_ray, light_dist, &tmp);
	if (is_hit)
	{
		printf("\tObstacle at x=%.3f y=%.3f z=%.3f\n", tmp.point.x, tmp.point.y, tmp.point.z);
		rec->t = tmp.t;
		rec->point = tmp.point;
	}
	else
		printf("\tNo obstacle\n");
	return (is_hit);
}

static t_vec3	apply_ambient(t_vec3 color, t_vec3 ambient)
{
	return ((t_vec3){
		color.x * ambient.x / 255.0,
		color.y * ambient.y / 255.0,
		color.z * ambient.z / 255.0,
	});
}

static t_vec3	apply_diffuse(t_hit_record *rec, t_light *light)
{
	t_vec3	light_dir;
	double	diff;

	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	diff = fmax(0.0, vec3_dot(rec->normal, light_dir));
	return ((t_vec3){
		rec->color.x * light->color.x / 255.0 * diff * light->intensity,
		rec->color.y * light->color.y / 255.0 * diff * light->intensity,
		rec->color.z * light->color.z / 255.0 * diff * light->intensity,
	});
}

// Dummy function as the real one is buggy.
/*
static t_vec3	apply_specular(t_hit_record *rec, t_light *light, t_ray *ray)
{
	(void)rec;
	(void)light;
	(void)ray;
	return ((t_vec3){0, 0, 0});
}
*/
static t_vec3	apply_specular(t_hit_record *rec, t_light *light, t_ray *ray)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	dot_ln;
	double	spec;

	if (rec->specular <= 0.0)
		return (t_vec3){0, 0, 0};
	light_dir = vec3_normalize(vec3_sub(light->position, rec->point));
	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	dot_ln = vec3_dot(light_dir, rec->normal);
	reflect_dir = vec3_sub(vec3_scale(rec->normal, 2.0 * dot_ln), light_dir);
	spec = pow(fmax(0.0, vec3_dot(reflect_dir, view_dir)), rec->shininess);
	spec *= rec->specular * light->intensity;
	return (t_vec3){
		light->color.x * spec,
		light->color.y * spec,
		light->color.z * spec,
	};
}
static t_vec3	shade_verbose(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	bool		is_shadow;
	t_hit_record	tmp;
	t_vec3		result;
	t_list		*node;
	t_light		*light;

	tmp = *rec;
	rec->color = apply_ambient(rec->color, scene->ambient->color);
	result = rec->color;
	node = scene->lights;
	is_shadow = 1;
	while (node)
	{
		light = ((t_light *)((t_object *)node->content)->object);
		if (!in_shadow(rec, tmp, scene, light))
		{
			is_shadow = 0;
			result = vec3_add(result, apply_diffuse(&tmp, light));
			result = vec3_add(result, apply_specular(&tmp, light, ray));
		}
		node = node->next;
	}
	if (!is_shadow)
		rec->color = vec3_clamp(result, 0.0, 255.0);
	return (rec->color);
}

void	print_hit_info_debug(t_hit_record hc, t_scene *scene, t_ray *ray, double mouse_x, double mouse_y)
{
	t_hit_record	shade_hc;

	shade_hc = hc;
	printf("\nObject selected at address %p\n", hc.object);
	if (!hc.object)
		return ;
	printf("Object type: %d\n", hc.object->type);
	printf("Object hit coordinates: x=%.3f y=%.3f z=%.3f (d=%.3f) (u=%.3f, v=%.3f)\n",
		hc.point.x, hc.point.y, hc.point.z, hc.t, mouse_x, mouse_y);		
	printf("Object normal at hit: x=%.3f y=%.3f z=%.3f (d=%.3f)\n", hc.normal.x,
		hc.normal.y, hc.normal.z, vec3_length(hc.normal));
	printf("Object color at hit: x=%.3f y=%.3f z=%.3f\n", hc.color.x,
		hc.color.y, hc.color.z);
	// Shade info
	shade_verbose(&shade_hc, scene, ray);
	printf("Object shade coordinates: x=%.3f y=%.3f z=%.3f (d=%.3f)\n",
		shade_hc.point.x, shade_hc.point.y, shade_hc.point.z, shade_hc.t);		
	printf("Object color after shade: x=%.3f y=%.3f z=%.3f\n", shade_hc.color.x,
		shade_hc.color.y, shade_hc.color.z);
	
}
