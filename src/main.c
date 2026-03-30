#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

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
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
