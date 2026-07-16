/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_debug.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 23:19:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 00:50:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <stdio.h>

static void	print_vector(t_data *data, char *name, t_vec3 vec, t_vec3 pos)
{
	char				buf[128];

	sprintf(buf, "%s: %.2f %.2f %.2f", name, vec.x, vec.y, vec.z);
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, buf);
}

static void	print_fps(t_data *data, t_vec3 pos)
{
	static int			frame_count = 0;
	static double		last_time = 0;
	static double		fps = 0;
	double				now;
	char				buf[128];

	now = get_time();
	frame_count++;
	if (now - last_time >= 1.0)
	{
		fps = frame_count / (now - last_time);
		frame_count = 0;
		last_time = now;
	}
	sprintf(buf, "FPS: %.1f", fps);
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, buf);
}

void	add_debug(t_data *data)
{
	char			buf[128];

	print_fps(data, (t_vec3){{10, 20, 0}});
	sprintf(buf, "Render Scale: %d", data->render_scale);
	mlx_string_put(data->mlx, data->win, 10, 40,
		(mlx_color){.rgba = MLX_WHITE}, buf);
	print_vector(data, "POS", data->scene->cam->position,
		(t_vec3){{10, 60, 0}});
	print_vector(data, "DIR", data->scene->cam->direction,
		(t_vec3){{10, 80, 0}});
	sprintf(buf, "YAW: %.2f  PITCH: %.2f", data->scene->cam->props.yaw,
		data->scene->cam->props.pitch);
	mlx_string_put(data->mlx, data->win, 10, 100,
		(mlx_color){.rgba = MLX_WHITE}, buf);
	sprintf(buf, "BVH DISPLAY LEVEL: %d", data->scene->bvh_display_level);
	mlx_string_put(data->mlx, data->win, 10, 120,
		(mlx_color){.rgba = MLX_WHITE}, buf);
}
