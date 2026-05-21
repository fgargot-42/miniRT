/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:23:56 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 23:54:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "veclib.h"

static mlx_color	apply_selection_rim(t_vec3 shaded,
						t_hit_record *hc, t_ray *ray)
{
	static const t_vec3	rim_color = {80.0, 220.0, 255.0};
	t_vec3				view_dir;
	double				rim;
	t_vec3				result;

	view_dir = vec3_normalize(vec3_scale(ray->direction, -1.0));
	rim = fmax(.5, 1.0 - fmax(0.0, vec3_dot(hc->normal, view_dir)));
	rim = rim * rim * rim;
	result.x = shaded.x + rim_color.x * rim * 2.5;
	result.y = shaded.y + rim_color.y * rim * 2.5;
	result.z = shaded.z + rim_color.z * rim * 2.5;
	return (vec3_to_color(vec3_clamp(result, 0.0, 255.0)));
}

static t_vec3	draw_skybox(t_data *data, t_ray r)
{
	t_vec2 uv = get_uv(r.direction);
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	t_vec3 uvcol = uv_to_color(data->scene->skybox, uv, data->mlx);
	return (uvcol);
}

static mlx_color	get_pixel_color(int x, int y, t_data *data,
	int render_scale)
{
	mlx_color		color;
	t_ray			r;
	t_hit_record	hc;
	t_vec3			shaded;

	ft_bzero(&hc, sizeof(t_hit_record));
	r = camera_ray(data->scene->cam, x + render_scale / 2,
			y + render_scale / 2);
	color = vec3_to_color(data->scene->sky->color);
	if (data->scene->skybox)
		color = vec3_to_color(draw_skybox(data, r));
	if (hit_scene(data->scene, &r, T_MAX, &hc, data->bvh_display_depth))
	{
		if (!hc.object)
			return (vec3_to_color(hc.color));
		if (hc.object->type == OBJ_SPHERE)
		{
			// texture mapping
			//
			//
			t_vec2 uv = get_uv(hc.normal);
			uv.x = uv.x - floor(uv.x);
			uv.y = uv.y - floor(uv.y);
			t_vec3 uvcol = uv_to_color(&hc.object->sphere_tex, uv, data->mlx);
			t_vec3 base = shade(&hc, data->scene, &r);
			shaded = vec3_multiply(base, vec3_scale(uvcol, 1.0 / 255.0));
		}
		else
			shaded = shade(&hc, data->scene, &r);
		if (data->scene->selected && hc.object == data->scene->selected)
			color = apply_selection_rim(shaded, &hc, &r);
		else
			color = vec3_to_color(shaded);
	}
	return (color);
}

void	rt_draw_pixel(int x, int y, t_data *data, int render_scale)
{
	mlx_color		color;
	int				i;
	int				j;

	color = get_pixel_color(x, y, data, render_scale);
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
