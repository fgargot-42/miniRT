/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:51:47 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 23:45:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <pthread.h>

void	rt_draw_pixel(int x, int y, t_data *data, int render_scale);

static void	*draw_thread(void *data)
{
	int		x;
	int		y;

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
	mlx_clear_window(data->mlx, data->win, vec3_to_color((t_vec3){0, 0, 0}));
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	add_debug(data);
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
	mlx_clear_window(data->mlx, data->win, vec3_to_color((t_vec3){0, 0, 0}));
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	add_debug(data);
}

void	draw(t_data *data)
{
	if (NB_THREADS >= 2)
		draw_threads_create(data);
	else
		draw_single(data);
}
