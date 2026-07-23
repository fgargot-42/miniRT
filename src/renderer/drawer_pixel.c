/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:23:56 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/23 23:40:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static mlx_color	apply_selection_rim(t_vec3 shaded, t_hit_record *hc,
		t_ray *ray)
{
	static const t_vec3	rim_color = {{80.0, 220.0, 255.0}};
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

static mlx_color	get_pixel_color(int x, int y, t_scene *scene,
		int render_scale)
{
	mlx_color		color;
	t_ray			r;
	t_hit_record	hc;
	t_vec3			shaded;

	ft_bzero(&hc, sizeof(t_hit_record));
	r = camera_ray(scene->cam, x + render_scale / 2, y + render_scale / 2);
	color = vec3_to_color((t_vec3){{30, 30, 30}});
	if (hit_scene(scene, &r, T_MAX, &hc))
	{
		if (!hc.object)
			return (vec3_to_color(hc.color));
		shaded = shade(&hc, scene, &r);
		if (scene->selected && hc.object == scene->selected)
			color = apply_selection_rim(shaded, &hc, &r);
		else
			color = vec3_to_color(shaded);
	}
	return (color);
}

void	rt_draw_pixel(int x, int y, t_data *data)
{
	mlx_color	color;
	int			i;
	int			j;

	color = get_pixel_color(x, y, data->scene, data->render_scale);
	i = 0;
	while (i < data->render_scale)
	{
		j = 0;
		while (j < data->render_scale)
		{
			if ((x + i) < WIDTH && (y + j) < HEIGHT)
				mlx_set_image_pixel(data->mlx, data->img, x + i, y + j, color);
			j++;
		}
		i++;
	}
}
