/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:51:47 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/18 21:27:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT_bonus.h"
#include <pthread.h>

void		rt_draw_pixel(int x, int y, t_data *data, int render_scale);

void	draw_single(t_data *data)
{
	int	x;
	int	y;

	x = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			rt_draw_pixel(x, y, data, data->render_scale);
			y += data->render_scale;
		}
		x += data->render_scale;
	}
	mlx_clear_window(data->mlx, data->win, vec3_to_color((t_vec3){{0, 0, 0}}));
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

static void	*draw_thread(void *data)
{
	int	x;
	int	y;

	y = ((t_data *)data)->th_nb;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			rt_draw_pixel(x, y, (t_data *)data, 1);
			x++;
		}
		y += ((t_data *)data)->nb_threads;
	}
	return (NULL);
}

static void	draw_threads_create(t_data *data)
{
	int			i;
	t_data		*th_data[NB_THREADS];
	pthread_t	threads[NB_THREADS];

	i = 0;
	while (i < NB_THREADS)
	{
		th_data[i] = malloc(sizeof(t_data));
		memcpy(th_data[i], data, sizeof(t_data));
		th_data[i]->th_nb = i;
		th_data[i]->rng_seed = data->rng_seed + i * 7919;
		pthread_create(&threads[i], NULL, draw_thread, th_data[i]);
		i++;
	}
	i = 0;
	while (i < NB_THREADS)
	{
		pthread_join(threads[i], NULL);
		free(th_data[i]);
		i++;
	}
	ft_rand_r(&data->rng_seed);
	mlx_clear_window(data->mlx, data->win, vec3_to_color((t_vec3){{0, 0, 0}}));
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}

void	draw(t_data *data)
{
	double		render_start_time;
	double		render_finish_time;
	double		render_time_ms;

	render_start_time = get_time();
	if (NB_THREADS >= 2)
		draw_threads_create(data);
	else
		draw_single(data);
	render_finish_time = get_time();
	render_time_ms = (render_finish_time - render_start_time) * 1000.0;
	if (data->show_hud)
		add_debug(data, render_time_ms);
}
