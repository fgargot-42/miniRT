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

