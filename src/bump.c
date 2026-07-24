/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:34:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/24 23:38:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit_bonus.h"
#include "object_bonus.h"
#include "material.h"
#include "normal.h"
#include "veclib.h"
#include "libft.h"
#include "mlx.h"
#include <math.h>
#include <stdio.h>

double	get_bump_from_img(t_vec2 uv, t_object obj)
{
	mlx_color	pixel;
	double		amplitude;
	double		bump;
	int			x;
	int			y;

	amplitude = 0.0;
	if (!obj.material || !obj.material->normal_tex)
		return (0.0);
	if (obj.type == OBJ_SPHERE)
		amplitude = obj.radius * 0.02;
	if (obj.type == OBJ_TRIANGLE)
		amplitude = 0.02 * fmax(fmax(vec3_distance(obj.props.b, obj.props.c),
					vec3_length(obj.props.b)), vec3_length(obj.props.c));
	while (uv.x > 1.0)
		uv.x -= 1.0;
	while (uv.y > 1.0)
		uv.y -= 1.0;
	x = (1 - uv.x) * (obj.material->normal_tex->width - 1);
	y = uv.y * (obj.material->normal_tex->height - 1);
	pixel = mlx_get_image_pixel(obj.material->normal_tex->mlx,
			obj.material->normal_tex->data, x, y);
	bump = amplitude * (((double)pixel.r) / 255.0 - 0.5);
	return (bump);
}

static t_vec3	get_bump_gradient(t_bump bump, t_vec3 pu, t_vec3 pv,
	double (*height_fn)(t_vec2, t_object))
{
	double	eps[2];
	double	b00;
	double	bu;
	double	bv;
	t_vec3	n_prime;

	eps[0] = 1e-4;
	eps[1] = 1e-4;
	if (bump.obj->material && bump.obj->material->normal_tex)
	{
		eps[0] = 1.0 / bump.obj->material->normal_tex->width;
		eps[1] = 1.0 / bump.obj->material->normal_tex->height;
	}
	b00 = height_fn(bump.uv, *bump.obj);
	bu = (height_fn((t_vec2){{bump.uv.x + eps[0], bump.uv.y}}, *bump.obj)
			- b00) / eps[0];
	bv = (height_fn((t_vec2){{bump.uv.x, bump.uv.y + eps[1]}}, *bump.obj)
			- b00) / eps[1];
	n_prime = vec3_add(vec3_sub(
				bump.normal, vec3_scale(vec3_cross(bump.normal, pv), bu)),
			vec3_scale(vec3_cross(bump.normal, pu), bv));
	return (vec3_normalize(n_prime));
}

t_vec3	bump_normal_sphere(t_hit_record rec, t_vec2 uv,
	double (*height_fn)(t_vec2, t_object))
{
	t_bump	bump;
	t_vec3	pu;
	t_vec3	pv;
	t_vec3	n_prime;

	bump.cos_theta = cos(uv.y * M_PI);
	bump.sin_theta = sin(uv.y * M_PI);
	bump.cos_phi = cos(uv.x * 2.0 * M_PI);
	bump.sin_phi = sin(uv.x * 2.0 * M_PI);
	bump.normal = vec3_normalize(vec3_sub(rec.point, rec.object->position));
	bump.obj = rec.object;
	bump.uv = uv;
	pu = vec3_scale((t_vec3){{-bump.sin_phi, 0, bump.cos_phi}},
			2 * M_PI * rec.object->radius * bump.sin_theta);
	pv = vec3_scale((t_vec3){{bump.cos_theta * bump.cos_phi,
			-bump.sin_theta, bump.cos_theta * bump.sin_phi}},
			M_PI * rec.object->radius);
	n_prime = get_bump_gradient(bump, pu, pv, height_fn);
	return (n_prime);
}

static void	get_triangle_tangent_frame(t_object obj, t_vec3 *pu, t_vec3 *pv)
{
	double	du[2];
	double	dv[2];
	double	det;
	double	det_inv;
	t_vec3	normal;

	du[0] = obj.uv.tex_b.x - obj.uv.tex_a.x;
	du[1] = obj.uv.tex_c.x - obj.uv.tex_a.x;
	dv[0] = obj.uv.tex_b.y - obj.uv.tex_a.y;
	dv[1] = obj.uv.tex_c.y - obj.uv.tex_a.y;
	det = du[0] * dv[1] - du[1] * dv[0];
	if (fabs(det) < 1e-8)
	{
		normal = vec3_normalize(vec3_cross(obj.props.b, obj.props.c));
		*pu = vec3_normalize(obj.props.b);
		*pv = vec3_cross(normal, *pu);
		return ;
	}
	det_inv = 1.0 / det;
	*pu = vec3_scale(vec3_sub(vec3_scale(obj.props.b, dv[1]),
				vec3_scale(obj.props.c, dv[0])), det_inv);
	*pv = vec3_scale(vec3_sub(vec3_scale(obj.props.c, du[0]),
				vec3_scale(obj.props.b, du[1])), det_inv);
}

t_vec3	bump_normal_triangle(t_hit_record rec, t_vec2 uv,
	double (*height_fn)(t_vec2, t_object))
{
	t_bump	bump;
	t_vec3	pu;
	t_vec3	pv;
	t_vec3	n_prime;

	ft_bzero(&pu, sizeof(t_vec3));
	ft_bzero(&pv, sizeof(t_vec3));
	bump.normal = vec3_normalize(vec3_sub(rec.point, rec.object->position));
	bump.obj = rec.object;
	bump.uv = uv;
	get_triangle_tangent_frame(*rec.object, &pu, &pv);
	n_prime = get_bump_gradient(bump, pu, pv, height_fn);
	return (n_prime);
}
