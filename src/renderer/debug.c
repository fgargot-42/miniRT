/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 23:19:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 23:58:45 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <stdio.h>
#include <sys/time.h>

static double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	add_debug(t_data *data)
{
	static double	last_time = 0;
	static int		frame_count = 0;
	static double	fps = 0;

	char		buf[128];
	mlx_color	white;

	white = vec3_to_color((t_vec3){255, 255, 255});
	frame_count++;
	double now = get_time();
	if (now - last_time >= 1.0)
	{
		fps = frame_count / (now - last_time);
		frame_count = 0;
		last_time = now;
	}
	sprintf(buf, "FPS: %.1f", fps);
	mlx_string_put(data->mlx, data->win, 10, 20, white, buf);
	if (data->nb_threads > 1)
		sprintf(buf, "Multi-thread mode using %i threads", data->nb_threads);
	else
		sprintf(buf, "Single-thread mode");
	mlx_string_put(data->mlx, data->win, 10, 40, white, buf);
	sprintf(buf, "Render Scale: %d", data->render_scale);
	mlx_string_put(data->mlx, data->win, 10, 60, white, buf);
	sprintf(buf, "POS: %.2f %.2f %.2f",
		data->scene->cam->position.x,
		data->scene->cam->position.y,
		data->scene->cam->position.z);
	mlx_string_put(data->mlx, data->win, 10, 80, white, buf);
	sprintf(buf, "DIR: %.2f %.2f %.2f",
		data->scene->cam->direction.x,
		data->scene->cam->direction.y,
		data->scene->cam->direction.z);
	mlx_string_put(data->mlx, data->win, 10, 100, white, buf);
	sprintf(buf, "YAW: %.2f  PITCH: %.2f",
		data->scene->cam->yaw,
		data->scene->cam->pitch);
	mlx_string_put(data->mlx, data->win, 10, 120, white, buf);
}
