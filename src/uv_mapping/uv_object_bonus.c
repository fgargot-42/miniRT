/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_object_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 01:25:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 01:04:46 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_bonus.h"
#include "veclib.h"
#include <math.h>
#include <stdio.h>

t_vec2	get_sphere_uv(t_object *obj, t_vec3 point)
{
	double	a;
	double	b;
	t_vec3	vec;
	t_vec2	result;

	vec = vec3_normalize(vec3_sub(point, obj->position));
	vec = vec_apply_matrix(vec, obj->props.transform_axis);
	a = atan2(vec.z, vec.x);
	b = fmax(-1.0, fmin(1.0, vec.y));
	b = acos(vec.y);
	result.x = (a + M_PI) / (2 * M_PI);
	result.y = b / M_PI;
	return (result);
}

t_vec2	get_triangle_uv(t_object *obj, t_vec3 hit)
{
	t_vec3		obj_hit;
	double		dot[2][3];
	double		det;
	t_vec2		uv;

	obj_hit = vec3_sub(hit, obj->position);
	dot[0][0] = vec3_dot(obj->props.b, obj->props.b);
	dot[0][1] = vec3_dot(obj->props.b, obj->props.c);
	dot[0][2] = vec3_dot(obj->props.b, obj_hit);
	dot[1][1] = vec3_dot(obj->props.c, obj->props.c);
	dot[1][2] = vec3_dot(obj->props.c, obj_hit);
	det = dot[0][0] * dot[1][1] - pow(dot[0][1], 2);
	if (fabs(det) < 1e-10)
		return ((t_vec2){{0, 0}});
	uv.x = (dot[1][1] * dot[0][2] - dot[0][1] * dot[1][2]) / det;
	uv.y = (dot[0][0] * dot[1][2] - dot[0][1] * dot[0][2]) / det;
	uv = vec2_add(vec2_add(obj->uv.tex_a,
				vec2_scale(vec2_sub(obj->uv.tex_b, obj->uv.tex_a), uv.x)),
			vec2_scale(vec2_sub(obj->uv.tex_c, obj->uv.tex_a), uv.y));
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	return (uv);
}
