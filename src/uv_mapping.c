/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 18:21:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 19:14:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec2 get_uv(t_vec3 vec)
{
	double a;
	double b;
	t_vec2 result;

	a = atan2(vec.z, vec.x);
	b = fmax(-1.0, fmin(1.0, vec.y));
	b = acos(b);
	result.x = (a+M_PI) / (2 * M_PI);
	result.y = b / M_PI;
	return (result);
}


t_vec3 uv_to_color(t_texture *tex, t_vec2 uv, void *mlx)
{
    t_vec3 		col;
	mlx_color	pixel;
    int 		x;
    int 		y;

	x = (int)(uv.x * (tex->width - 1));
	y = (int)(uv.y * (tex->height - 1));
	pixel = mlx_get_image_pixel(mlx, tex->data, x, y);
	col.x = pixel.r / 255.0;
	col.y = pixel.g / 255.0;
	col.z = pixel.b / 255.0;
    return (col);
}

t_texture load_texture(char *path, void *mlx)
{
    t_texture tex;

	tex.data = mlx_new_image_from_file(mlx, path, &tex.width, &tex.height);
    if (!tex.data)
    {
        printf("Failed to load image\n");
        exit(1);
    }
    return (tex);
}
