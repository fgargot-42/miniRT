/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_display.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 21:16:14 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/31 18:51:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <mlx.h>
#include <stdlib.h>

void	init_display(char *rt_file, t_data *data)
{
	mlx_window_create_info	info;

	data->render_scale = 1;
	data->dragging_slider = -1;
	data->show_hud = true;
	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	mlx_set_font(data->mlx, "resources/font.ttf");
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 16.0f);
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
}

void	destroy_display(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	if (data->editor)
		mlx_destroy_window(data->mlx, data->editor);
	mlx_destroy_context(data->mlx);
}
