#include "miniRT.h"

static int in_shadow(t_hit_record *rec, t_scene *scene)
{
    t_ray   shadow_ray;
    t_vec3  to_light;
    double  light_dist;
    t_hit_record tmp;

    to_light = vec_sub(scene->light.position, rec->point);
    light_dist = vec_length(to_light);
    shadow_ray.origin = rec->point;
    shadow_ray.direction = vec_normalize(to_light);
    return (hit_scene(scene, &shadow_ray, light_dist, &tmp));
}

static t_vec3	apply_ambient(t_vec3 color, t_vec3 ambient) // multiplie color rgb par ambient lightning color
{
	return (t_vec3){
		color.x * ambient.x / 255.0,
		color.y * ambient.y / 255.0,
		color.z * ambient.z / 255.0,
	};
}

static t_vec3	apply_diffuse(t_hit_record *rec, t_light *light) // apply light to hit rec
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

t_vec3 shade(t_hit_record *rec, t_scene *scene)
{
    t_vec3 ambient = apply_ambient(rec->color, scene->ambient);
    if (in_shadow(rec, scene))
        return (ambient);
    t_vec3 diffuse = apply_diffuse(rec, &scene->light);
    return (t_vec3){
        fmin(ambient.x + diffuse.x, 255.0),
        fmin(ambient.y + diffuse.y, 255.0),
        fmin(ambient.z + diffuse.z, 255.0),
    };
}
