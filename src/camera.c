#include "miniRT.h"

static void apply_scale(double *u, double *v, double fov)
{
	double scale;
	scale = tan((fov / 2.0) * M_PI / 180.0);

	*u = *u * scale;
	*v = *v * scale;
}

t_ray	camera_ray(t_camera cam, int x, int y)
{
	double u;
	double v;
	t_ray ray;

	u = (x / (double)WIDTH) * 2.0 - 1.0;
	v =  (y /(double)HEIGHT) * 2.0 - 1.0; // range -1 -> 1;
	u = u * ((double)WIDTH / HEIGHT); // aspect ratio fix
	apply_scale(&u, &v, cam.fov);
	ray.origin = cam.position;
	ray.direction = vec_normalize((t_vec3){u, -v, 1.0}); //-v world y = up
	return (ray);
}

