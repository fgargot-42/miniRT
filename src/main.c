#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

#define T_MIN 0.001
#define T_MAX 1e9

typedef struct s_scene
{
	t_sphere sphere_list[1];
	t_camera cam;
}	t_scene;

mlx_color   vec3_to_color(t_vec3 v)
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

void	draw(t_data *data)
{
	t_sphere sphere;
	t_camera cam;
	t_ray r;
	t_hit_record hc;

	cam.position = (t_vec3){0, 0, 0};
	cam.fov = 90;
	sphere.center = (t_vec3){0, 0, 5};
	sphere.color = (t_vec3){255, 0, 0};
	sphere.radius = 5;

	int x;
	int y;
	x = 0;

	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			r = camera_ray(cam, x, y);
			if (hit_sphere(&sphere, &r, T_MIN, T_MAX, &hc))
				mlx_set_image_pixel(data->mlx, data->img, x, y, vec3_to_color(hc.color));
			y++;
		}
		x++;
	}
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

	init(&data);
	draw(&data);
	attach_hooks(&data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
