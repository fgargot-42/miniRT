/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:51:47 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 21:45:20 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <pthread.h>


static mlx_color	apply_selection_rim(t_vec3 shaded,
						t_hit_record *hc, t_ray *ray)
{
	static const t_vec3	rim_color = {80.0, 220.0, 255.0};
	t_vec3				view_dir;
	double				rim;
	t_vec3				result;
 
	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	rim = 1.0 - fmax(0.0, vec3_dot(hc->normal, view_dir));
	rim = rim * rim * rim;
	result.x = shaded.x + rim_color.x * rim * 2.5;
	result.y = shaded.y + rim_color.y * rim * 2.5;
	result.z = shaded.z + rim_color.z * rim * 2.5;
	return (vec3_to_color(vec3_clamp(result, 0.0, 255.0)));
}

static void	rt_draw_pixel(int x, int y, t_data *data, int render_scale)
{
	t_ray			r;
	t_hit_record	hc;
	mlx_color		color;
	int				i;
	int				j;
	t_vec3			shaded;

	r = camera_ray(data->scene->cam, x + render_scale / 2,
			y + render_scale / 2);

	if (hit_scene(data->scene, &r, T_MAX, &hc))
	{
		shaded = shade(&hc, data->scene, &r);
		if (data->scene->selected && hc.object == data->scene->selected)
			color = apply_selection_rim(shaded, &hc, &r);
		else
			color = vec3_to_color(shaded);
	}
	else
		color = vec3_to_color(data->scene->sky->color);

	i = 0;
	while (i < render_scale)
	{
		j = 0;
		while (j < render_scale)
		{
			if ((x + i) < WIDTH && (y + j) < HEIGHT)
				mlx_set_image_pixel(data->mlx, data->img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}

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
