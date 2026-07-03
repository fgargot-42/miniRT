/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:34:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/02 20:35:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
#include "matlib.h"

t_vec3	apply_normal_map(t_hit_record *rec)
{
	t_vec3	new_normal;
	t_vec3	normal;
	t_vec2	delta_uv[2];
	t_vec3	edge[2];
	t_vec3	tans[2];
	t_mat3	TBN;
	double	f;
	
	normal = triangle_uv_to_color(rec->object, rec->point);
	normal = vec3_sub(vec3_scale(normal, 1.0 / 127.5), (t_vec3){{1.0, 1.0, 1.0}});
	normal = vec3_normalize(normal);
	// 
	delta_uv[0] = vec2_sub(rec->object->uv.tex_b, rec->object->uv.tex_a);
	delta_uv[1] = vec2_sub(rec->object->uv.tex_c, rec->object->uv.tex_a);
	edge[0] = vec3_sub(rec->object->props.b, rec->object->props.a);
	edge[1] = vec3_sub(rec->object->props.c, rec->object->props.a);
	f = 1.0 / (delta_uv[0].x *delta_uv[1].y - delta_uv[0].y * delta_uv[1].x);
	tans[0] = vec3_scale(vec3_sub(vec3_scale(edge[0], delta_uv[1].y),
				vec3_scale(edge[1], delta_uv[0].y)), f);
	tans[1] = vec3_scale(vec3_sub(vec3_scale(edge[1], delta_uv[0].x),
				vec3_scale(edge[0], delta_uv[1].x)), f);
	tans[0] = vec3_normalize(vec3_sub(tans[0],
				vec3_scale(rec->normal, vec3_dot(rec->normal, tans[0]))));
	TBN = init_mat3(tans[0], tans[1], rec->normal);
	TBN = transpose_mat3(TBN);
	new_normal = vec3_normalize(mat3_vec_mult(TBN, normal));
	return (new_normal);
}
