#include "miniRT.h"
int	hit_sphere(t_sphere *sphere, t_ray *ray, double t_min, double t_max, t_hit_record *rec)
{
	t_vec3 oc;
	double a;
	double b;
	double c;
	double discriminant;
	double sqrt_d;
	double root;

	oc = vec_sub(ray->origin, sphere->center);

	a = vec_dot(ray->direction, ray->direction);
	b = 2.0 * vec_dot(oc, ray->direction);
	c = vec_dot(oc, oc) - sphere->radius * sphere->radius;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);

	sqrt_d = sqrt(discriminant);
	root = (-b - sqrt_d) / (2.0 * a);

	if (root < t_min || root > t_max)
	{
		root = (-b + sqrt_d) / (2.0 * a);
		if (root < t_min || root > t_max)
			return (0);

	}

	rec->t = root;
	rec->point = ray_at(*ray, rec->t);
	rec->normal = vec_normalize(vec_sub(rec->point, sphere->center));
	rec->color = sphere->color;
	rec->object.sphere = sphere;

	return (1);
}


int	hit_plane(t_plane *plane, t_ray *ray, double t_min, double t_max, t_hit_record *rec)
{
	double	d;
	double	t;
	int		x;
	int		z;

	d = vec_dot(ray->direction, plane->normal);
	if (fabs(d) < 1e-6)
		return (0);
	t = vec_dot(vec_sub(plane->point, ray->origin), plane->normal) / d;
	if (t < t_min || t > t_max)
		return (0);
	rec->t = t;
	rec->point = ray_at(*ray, t);
	rec->normal = plane->normal;
	rec->object.plane = plane;
	if (plane->checker)
	{
		x = (int)floor(rec->point.x);
		z = (int)floor(rec->point.z);
		if ((x + z) % 2 == 0)
			rec->color = (t_vec3){255, 255, 255};
		else
			rec->color = (t_vec3){30, 30, 30};
	}
	else
		rec->color = plane->color;
	return (1);
}

int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_min, double t_max, t_hit_record *rec)
{
	t_vec3 oc;
	t_vec3 cd;
	double a;
	double b;
	double c;
	double discriminant;
	double sqrt_d;
	double root;

	oc = vec_multiply(vec_sub(ray->origin, cyl->center), vec_sub((t_vec3){1, 1, 1}, cyl->axis));
	cd = vec_multiply(ray->direction, vec_sub((t_vec3){1, 1, 1}, cyl->axis));

	a = vec_dot(cd, cd);
	b = 2.0 * vec_dot(oc, cd);
	c = vec_dot(oc, oc) - cyl->radius * cyl->radius;

	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (0);

	sqrt_d = sqrt(discriminant);
	root = (-b - sqrt_d) / (2.0 * a);

	if (root < t_min || root > t_max)
	{
		root = (-b + sqrt_d) / (2.0 * a);
		if (root < t_min || root > t_max)
			return (0);

	}

	rec->t = root;
	rec->point = ray_at(*ray, rec->t);
	rec->normal = vec_normalize(vec_sub(rec->point, cyl->center));
	rec->color = cyl->color;
	rec->object.cylinder = cyl;

	return (1);
}

static int hit_list(t_hit_list h, t_ray *ray, double *closest, t_hit_record *rec)
{
	t_hit_record temp;
	int				hit;

	hit = 0;
	while (h.list)
	{
		if (h.hit_fn(h.list->content, ray, T_MIN, *closest, &temp))
		{
			hit = 1;
			*closest = temp.t;
			*rec = temp;
		}
		h.list = h.list->next;
	}
	return (hit);
}

int	hit_scene(t_scene *scene, t_ray *ray, double t_max, t_hit_record *rec)
{
    int     hit;
    double  closest;

    hit = 0;
    closest = t_max;
    hit |= hit_list((t_hit_list){scene->spheres,   (void *)hit_sphere},   ray, &closest, rec);
    hit |= hit_list((t_hit_list){scene->planes,    (void *)hit_plane},    ray, &closest, rec);
    hit |= hit_list((t_hit_list){scene->cylinder, (void *)hit_cylinder}, ray, &closest, rec);
    return (hit);
}
