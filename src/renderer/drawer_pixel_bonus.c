/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_pixel_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 23:23:56 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:37:33 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hit_bonus.h"
#include "miniRT_bonus.h"
#include "uv_bonus.h"
#include "veclib.h"
#include "normal_bonus.h"
#include <time.h>

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

static void	apply_uv(t_hit_record *hc, bool is_bump)
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
			if (is_bump)
				hc->normal = bump_normal_triangle(*hc, uv, get_bump_from_img);
		}
	}
	if (hc->object->type == OBJ_SPHERE && hc->object->material->color_tex)
	{
		hc->color = uv_to_color(hc->object,
				hc->object->material->color_tex, uv);
		if (is_bump)
			hc->normal = bump_normal_sphere(*hc, uv, get_bump_from_img);
	}
}

t_vec3	rt_cast(t_scene *scene, t_ray *r, t_object *obj_from, int depth)
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
		apply_uv(&hc, scene->bump);
		if (obj_from == hc.object)
			r->refraction = 1;
		shaded = shade(&hc, scene, r);
		if (scene->selected && hc.object == scene->selected && depth == 0)
			color = apply_selection_rim(shaded, &hc, r);
		else
			color = shaded;
	}
	return (color);
}

static mlx_color	get_pixel_color(int x, int y, t_scene *scene,
		bool anti_aliasing)
{
	static bool	is_rng_init = false;
	t_vec3		color;
	t_ray		r;
	int			i;

	if (!is_rng_init)
		srand(time(NULL));
	is_rng_init = true;
	i = 0;
	color = (t_vec3){{0, 0, 0}};
	while (i < RAYS_PER_PIXEL)
	{
		if (anti_aliasing)
			r = camera_ray(scene->cam, x + (double)rand() / (double)RAND_MAX,
					y + (double)rand() / (double)RAND_MAX);
		else
			r = camera_ray(scene->cam, x, y);
		r.refraction = 1.0;
		color = vec3_add(color, rt_cast(scene, &r, NULL, 0));
		i++;
		if (!anti_aliasing)
			break ;
	}
	color = vec3_scale(color, 1.0 / i);
	return (vec3_to_color(color));
}

void	rt_draw_pixel(int x, int y, t_data *data, int render_scale)
{
	mlx_color	color;
	int			i;
	int			j;

	color = get_pixel_color(x + render_scale / 2, y + render_scale / 2,
			data->scene,
			data->scene->anti_aliasing && !data->r_click_hold
			&& !data->w_click_hold && data->ui.dragging_slider == -1);
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
