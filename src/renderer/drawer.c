/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:51:47 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/22 01:39:45 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT.h"

void		rt_draw_pixel(int x, int y, t_data *data, int render_scale);

void	draw(t_data *data)
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
	add_debug(data);
}
