/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 20:22:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/11 19:46:59 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <mlx.h>
#include <stdlib.h>

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

void	init(char *rt_file, t_data *data)
{
	mlx_window_create_info	info;

	data->render_scale = 1;
	data->dragging_slider = -1;
	data->nb_threads = NB_THREADS;
	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	init_scene(rt_file, data);
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
	mlx_set_image_pixel(data->mlx, data->img, 0, 0, vec3_to_color((t_vec3){{.x=0, .y=0,
			.z=0}}));
}

static void	destroy_all(t_data *data)
{
	free_scene(data->scene, data->mlx);
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	if (data->editor)
		mlx_destroy_window(data->mlx, data->editor);
	mlx_destroy_context(data->mlx);
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
	data.scene = ft_calloc(1, sizeof(t_scene));
	if (!data.scene)
	{
		fprintf(stderr, "Failed to allocate scene\n");
		return (1);
	}
	init(argv[1], &data);
#if DEBUG
	// ft_lstiter(data.scene->objects, print_object);
	// ft_lstiter(data.scene->lights, print_object);
	// print_sky(data.scene->sky);
#endif
	draw(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
