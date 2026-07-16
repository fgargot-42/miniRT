/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_display_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 21:21:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 21:21:55 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <mlx.h>
#include <stdlib.h>

void	init_display(char *rt_file, t_data *data)
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
	mlx_set_image_pixel(data->mlx, data->img, 0, 0,
		vec3_to_color((t_vec3){{.x = 0, .y = 0, .z = 0}}));
}

void	destroy_display(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	if (data->editor)
		mlx_destroy_window(data->mlx, data->editor);
	mlx_destroy_context(data->mlx);
}
