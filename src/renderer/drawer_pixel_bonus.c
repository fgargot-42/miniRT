/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_pixel_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:23:56 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/28 01:21:08 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hit_bonus.h"
#include "miniRT_bonus.h"
#include "uv.h"
#include "veclib.h"
#include "normal.h"
#include <assert.h>

static t_vec3	apply_selection_rim(t_vec3 shaded, t_hit_record *hc,
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
	return (vec3_clamp(result, 0.0, 255.0));
}

static void	apply_uv(t_hit_record *hc)
{
	t_vec2			uv;

	if (!hc->object->material)
		return ;
	uv = get_uv(hc->object, hc->point);
	if (hc->object->type == OBJ_TRIANGLE)
	{
		if (hc->object->material->spec_tex)
			hc->specular = uv_to_color(hc->object,
					hc->object->material->spec_tex, uv).x / 255.0;
		if (hc->object->material->color_tex)
		{
			hc->color = uv_to_color(hc->object,
					hc->object->material->color_tex, uv);
			//hc->normal = bump_normal_triangle(*hc, uv, get_bump_from_img);
		}
	}
	if (hc->object->type == OBJ_SPHERE && hc->object->material->color_tex)
	{
		hc->color = uv_to_color(hc->object, hc->object->material->color_tex, uv);
		hc->normal = bump_normal_sphere(*hc, uv, get_bump_from_img);
	}
}

t_vec3	rt_cast(t_scene *scene, t_ray *r, int depth)
{
	t_vec3			color;
	t_hit_record	hc;
	t_vec3			shaded;

	ft_bzero(&hc, sizeof(t_hit_record));
	hc.depth = depth;
	color = scene->sky->color;
	if (scene->skybox)
		color = draw_skybox(scene, *r);
	if (hit_scene(scene, r, T_MAX, &hc))
	{
		if (!hc.object)
			return (hc.color);
		apply_uv(&hc);
		shaded = shade(&hc, scene, r);
		if (scene->selected && hc.object == scene->selected && depth == 0)
			color = apply_selection_rim(shaded, &hc, r);
		else
			color = shaded;
	}
	return (color);
}

static mlx_color	get_pixel_color(int x, int y, t_scene *scene,
		int render_scale)
{
	t_vec3		color;
	t_ray		r;

	r = camera_ray(scene->cam, x + render_scale / 2, y + render_scale / 2);
	color = rt_cast(scene, &r, 0);
	return (vec3_to_color(color));
}

void	rt_draw_pixel(int x, int y, t_data *data, int render_scale)
{
	mlx_color	color;
	int			i;
	int			j;

	color = get_pixel_color(x, y, data->scene, render_scale);
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
