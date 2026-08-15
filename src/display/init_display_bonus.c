/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_display_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 21:21:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/15 02:03:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <mlx.h>
#include <stdlib.h>

static void	init_ui(t_ui_info *ui)
{
	ui->dragging_slider = -1;
	ui->selected_light = 0;
	ui->last_mouse_x = SLD_X + SLD_W / 2;
	ui->last_mouse_y = SLD_W + SLD_H / 2;
}

void	init_display(char *rt_file, t_data *data)
{
	mlx_window_create_info	info;

	init_ui(&data->ui);
	data->render_scale = 1;
	data->show_hud = true;
	data->nb_threads = NB_THREADS;
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
