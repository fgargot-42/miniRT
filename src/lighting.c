#include "miniRT.h"
#include <math.h>

static int	in_shadow(t_hit_record *rec, t_scene *scene, t_light *light)
{
	t_ray			shadow_ray;
	t_vec3			to_light;
	double			light_dist;
	t_hit_record	tmp;

	to_light = vec_sub(light->position, rec->point);
	light_dist = vec_length(to_light);
	shadow_ray.origin = rec->point;
	shadow_ray.direction = vec_normalize(to_light);
	return (hit_scene(scene, &shadow_ray, light_dist, &tmp));
}

static t_vec3	apply_ambient(t_vec3 color, t_vec3 ambient)
{
	return (t_vec3){
		color.x * ambient.x / 255.0,
		color.y * ambient.y / 255.0,
		color.z * ambient.z / 255.0,
	};
}

static t_vec3	apply_diffuse(t_hit_record *rec, t_light *light)
{
	t_vec3	light_dir;
	double	diff;

	light_dir = vec_normalize(vec_sub(light->position, rec->point));
	diff = fmax(0.0, vec_dot(rec->normal, light_dir));
	return (t_vec3){
		rec->color.x * light->color.x / 255.0 * diff * light->intensity,
		rec->color.y * light->color.y / 255.0 * diff * light->intensity,
		rec->color.z * light->color.z / 255.0 * diff * light->intensity,
	};
}

static t_vec3	apply_specular(t_hit_record *rec, t_light *light, t_ray *ray)
{
	t_vec3	light_dir;
	t_vec3	view_dir;
	t_vec3	reflect_dir;
	double	dot_ln;
	double	spec;

	if (rec->specular <= 0.0)
		return (t_vec3){0, 0, 0};
	light_dir = vec_normalize(vec_sub(light->position, rec->point));
	view_dir = vec_normalize(vec_scale(ray->direction, -1.0));
	dot_ln = vec_dot(light_dir, rec->normal);
	reflect_dir = vec_sub(vec_scale(rec->normal, 2.0 * dot_ln), light_dir);
	spec = pow(fmax(0.0, vec_dot(reflect_dir, view_dir)), rec->shininess);
	spec *= rec->specular * light->intensity;
	return (t_vec3){
		light->color.x * spec,
		light->color.y * spec,
		light->color.z * spec,
	};
}

t_vec3	shade(t_hit_record *rec, t_scene *scene, t_ray *ray)
{
	t_vec3		result;
	t_vec3		diffuse;
	t_vec3		specular;
	t_list		*node;
	t_light		*light;

	result = apply_ambient(rec->color, scene->ambient);
	node = scene->lights;
	while (node)
	{
		light = (t_light *)node->content;
		if (!in_shadow(rec, scene, light))
		{
			diffuse = apply_diffuse(rec, light);
			specular = apply_specular(rec, light, ray);
			result.x += diffuse.x + specular.x;
			result.y += diffuse.y + specular.y;
			result.z += diffuse.z + specular.z;
		}
		node = node->next;
	}
	return (t_vec3){
		fmin(result.x, 255.0),
		fmin(result.y, 255.0),
		fmin(result.z, 255.0),
	};
}
