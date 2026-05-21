/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 20:22:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/22 00:04:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

void	clear_image(t_data *fdf)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			mlx_set_image_pixel(fdf->mlx, fdf->img, x, y,
				(mlx_color){.rgba = 0x000000FF});
			x++;
		}
		y++;
	}
}

void	init(t_data *data)
{
	mlx_window_create_info	info;

	data->render_scale = 1;
	data->dragging_slider = -1;
	data->nb_threads = NB_THREADS;
	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	info = (mlx_window_create_info){.title = "miniRT", .width = WIDTH,
		.height = HEIGHT};
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		exit(1);
	mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
	mlx_mouse_get_pos(data->mlx, &data->last_mouse_x, &data->last_mouse_y);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		exit(1);
}

static void	destroy_all(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	if (data->editor)
		mlx_destroy_window(data->mlx, data->editor);
	mlx_destroy_context(data->mlx);
	free_scene(data->scene);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		ft_putstr_fd("Error\n1 argument expected\n./miniRT <scene.rt>\n", 2);
		return (1);
	}
	ft_bzero(&data, sizeof(t_data));
	data.scene = malloc(sizeof(t_scene));
	if (!data.scene)
	{
		fprintf(stderr, "Failed to allocate scene\n");
		return (1);
	}
	init(&data);
	init_scene(argv[1], &data);
	data.scene->skybox = ft_calloc(1, sizeof(t_texture));
	*data.scene->skybox = load_texture("textures/8k_stars.jpg", data.mlx);
#if DEBUG
	//ft_lstiter(data.scene->objects, print_object);
	ft_lstiter(data.scene->lights, print_object);
	print_sky(data.scene->sky);
#endif
	draw_single(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
