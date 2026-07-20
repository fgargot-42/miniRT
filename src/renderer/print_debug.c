/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:32:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 23:56:48 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"

t_vec3	apply_ambient(t_vec3 color, t_vec3 ambient);
t_vec3	apply_diffuse(t_hit_record *rec, t_object *light);

static int	in_shadow(t_hit_record *rec, t_hit_record tmp, t_scene *scene,
	t_object *light)
{
	t_ray			shadow_ray;
	t_vec3			to_light;
	double			light_dist;
	bool			is_hit;

	to_light = vec3_sub(light->position, tmp.point);
	light_dist = vec3_length(to_light);
	shadow_ray.origin = tmp.point;
	shadow_ray.direction = vec3_normalize(to_light);
	printf("Check obstacle to light: x=%.3f y=%.3f z=%.3f (d=%.3f):\n",
		light->position.x, light->position.y, light->position.z, light_dist);
	printf("\tRay direction: x=%.3f y=%.3f z=%.3f\n", shadow_ray.direction.x,
		shadow_ray.direction.y, shadow_ray.direction.z);
	is_hit = hit_scene(scene, &shadow_ray, light_dist, &tmp);
	if (is_hit)
	{
		printf("\tObstacle at x=%.3f y=%.3f z=%.3f\n", tmp.point.x,
			tmp.point.y, tmp.point.z);
		rec->t = tmp.t;
		rec->point = tmp.point;
	}
	else
		printf("\tNo obstacle\n");
	return (is_hit);
}

static t_vec3	shade_verbose(t_hit_record *rec, t_scene *scene)
{
	t_hit_record	tmp;

	tmp = *rec;
	rec->color = apply_ambient(rec->color, scene->ambient->color);
	if (in_shadow(rec, tmp, scene, scene->light))
		return (rec->color);
	rec->color = vec3_add(rec->color, apply_diffuse(&tmp, scene->light));
	rec->color = vec3_clamp(rec->color, 0.0, 255.0);
	return (rec->color);
}

static void	print_rotation_info_debug(t_object *obj)
{
	t_vec3	axis;
	t_mat3	mat;

	axis = obj->direction;
	mat = obj->props.transform_axis;
	printf("Object axis: x=%.3f y=%.3f z=%.3f\n", axis.x, axis.y, axis.z);
	printf("Object rotation matrix:\n[ %.3f\t%.3f\t%.3f ]\n",
		mat.mat[0][0], mat.mat[0][1], mat.mat[0][2]);
	printf("[ %.3f\t%.3f\t%.3f ]\n",
		mat.mat[1][0], mat.mat[1][1], mat.mat[1][2]);
	printf("[ %.3f\t%.3f\t%.3f ]\n",
		mat.mat[2][0], mat.mat[2][1], mat.mat[2][2]);
	axis = vec_apply_rotation_z(axis, mat);
	printf("\tObject after rotation: x=%.3f y=%.3f z=%.3f\n",
		axis.x, axis.y, axis.z);
	axis = vec_reverse_rotation(axis, mat);
	printf("\tObject rotation back: x=%.3f y=%.3f z=%.3f\n",
		axis.x, axis.y, axis.z);
}

void	print_hit_info_debug(t_hit_record hc, t_scene *scene, t_vec2 mouse_pos)
{
	t_hit_record	shade_hc;

	shade_hc = hc;
	printf("\nObject selected at address %p\n", hc.object);
	if (!hc.object)
		return ;
	printf("Object type: %d\n", hc.object->type);
	if (hc.object->type == OBJ_CYLINDER)
		print_rotation_info_debug(hc.object);
	printf("Object hit coordinates: x=%.3f y=%.3f z=%.3f",
		hc.point.x, hc.point.y, hc.point.z);
	printf("(d=%.3f) (u=%.3f, v=%.3f)\n", hc.t, mouse_pos.x, mouse_pos.y);
	printf("Object normal at hit: x=%.3f y=%.3f z=%.3f (d=%.3f)\n", hc.normal.x,
		hc.normal.y, hc.normal.z, vec3_length(hc.normal));
	printf("Object color at hit: x=%.3f y=%.3f z=%.3f\n", hc.color.x,
		hc.color.y, hc.color.z);
	shade_verbose(&shade_hc, scene);
	printf("Object shade coordinates: x=%.3f y=%.3f z=%.3f (d=%.3f)\n",
		shade_hc.point.x, shade_hc.point.y, shade_hc.point.z, shade_hc.t);
	printf("Object color after shade: x=%.3f y=%.3f z=%.3f\n", shade_hc.color.x,
		shade_hc.color.y, shade_hc.color.z);
}
