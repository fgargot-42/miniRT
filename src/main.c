#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

mlx_color	vec3_to_color(t_vec3 v)
{
	return (mlx_color){
		.r = (int)v.x,
		.g = (int)v.y,
		.b = (int)v.z,
		.a = 255
	};
}

void	init(t_data *data)
{
	mlx_window_create_info	info;

	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	info = (mlx_window_create_info)
	{
		.title = "miniRT",
		.width = WIDTH,
		.height = HEIGHT,
	};
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		exit(1);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		exit(1);
}

void	init_scene(t_scene *scene)
{
	t_sphere	*s;
	t_plane		*p;
	t_cylinder	*c;

	ft_bzero(scene, sizeof(t_scene));

	// Camera
	scene->cam.position = (t_vec3){0, 2, -5};
	scene->cam.fov = 80;

	// Warm key light from top left
	scene->light.position = (t_vec3){-4, 8, 2};
	scene->light.intensity = 0.8;
	scene->light.color = (t_vec3){255, 240, 200};

	// Soft ambient
	scene->ambient = (t_vec3){15, 15, 25};

	// Checkered floor
	p = malloc(sizeof(t_plane));
	p->point = (t_vec3){0, -1.5, 0};
	p->normal = (t_vec3){0, 1, 0};
	p->color = (t_vec3){200, 200, 200};
	p->checker = 1;
	ft_lstadd_back(&scene->planes, ft_lstnew(p));

	// Big center sphere - matte white
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){0, 0, 6};
	s->radius = 1.5;
	s->color = (t_vec3){230, 230, 230};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Left sphere - deep blue
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){-3, -0.5, 5};
	s->radius = 1.0;
	s->color = (t_vec3){50, 80, 200};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Right sphere - coral red
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){3, -0.5, 5};
	s->radius = 1.0;
	s->color = (t_vec3){220, 80, 60};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Small sphere on top of center - gold
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){0, 2.2, 6};
	s->radius = 0.5;
	s->color = (t_vec3){255, 200, 50};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Tiny sphere front left - mint green
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){-1.5, -1.0, 3};
	s->radius = 0.35;
	s->color = (t_vec3){100, 220, 150};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Tiny sphere front right - purple
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){1.5, -1.0, 3};
	s->radius = 0.35;
	s->color = (t_vec3){180, 80, 220};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Cylinder
	c = malloc(sizeof(t_cylinder));
	c->center = (t_vec3){5, 4, 6};
	c->axis = (t_vec3){0, 1, 0};
	c->radius = 1;
	c->height = 4;
	c->color = (t_vec3){120, 180, 160};
	ft_lstadd_back(&scene->cylinder, ft_lstnew(c));
}

void	draw(t_data *data, t_scene *scene)
{
	t_ray			r;
	t_hit_record	hc;
	int				x;
	int				y;

	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			r = camera_ray(scene->cam, x, y);
			if (hit_scene(scene, &r, T_MAX, &hc))
				mlx_set_image_pixel(data->mlx, data->img, x, y, vec3_to_color(shade(&hc, scene)));
			y++;
		}
		x++;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

static void	destroy_all(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_context(data->mlx);
}

int	main(void)
{
	t_data	data;
	t_scene	scene;

	init_scene(&scene);
	init(&data);
	draw(&data, &scene);
	attach_hooks(&data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
