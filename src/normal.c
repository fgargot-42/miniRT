/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/02 20:34:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/22 00:42:52 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"
#include "matlib.h"

//t_vec3	apply_normal_map(t_hit_record *rec)
//{
//	t_vec3	new_normal;
//	t_vec3	normal;
//	t_vec2	delta_uv[2];
//	t_vec3	edge[2];
//	t_vec3	tans[2];
//	t_mat3	TBN;
//	double	f;
//	
//	normal = triangle_uv_to_color(rec->object, rec->point);
//	normal = vec3_sub(vec3_scale(normal, 1.0 / 127.5), (t_vec3){{1.0, 1.0, 1.0}});
//	normal = vec3_normalize(normal);
//	// 
//	delta_uv[0] = vec2_sub(rec->object->uv.tex_b, rec->object->uv.tex_a);
//	delta_uv[1] = vec2_sub(rec->object->uv.tex_c, rec->object->uv.tex_a);
//	edge[0] = vec3_sub(rec->object->props.b, rec->object->props.a);
//	edge[1] = vec3_sub(rec->object->props.c, rec->object->props.a);
//	f = 1.0 / (delta_uv[0].x *delta_uv[1].y - delta_uv[0].y * delta_uv[1].x);
//	tans[0] = vec3_scale(vec3_sub(vec3_scale(edge[0], delta_uv[1].y),
//				vec3_scale(edge[1], delta_uv[0].y)), f);
//	tans[1] = vec3_scale(vec3_sub(vec3_scale(edge[1], delta_uv[0].x),
//				vec3_scale(edge[0], delta_uv[1].x)), f);
//	tans[0] = vec3_normalize(vec3_sub(tans[0],
//				vec3_scale(rec->normal, vec3_dot(rec->normal, tans[0]))));
//	TBN = init_mat3(tans[0], tans[1], rec->normal);
//	TBN = transpose_mat3(TBN);
//	new_normal = vec3_normalize(mat3_vec_mult(TBN, normal));
//	return (new_normal);
//}

typedef struct	s_bump
{
	t_object	*obj;
	t_vec2		uv;
	t_vec3		normal;
	double		cos_theta;
	double		sin_theta;
	double		cos_phi;
	double		sin_phi;
}	t_bump;

static t_vec3	get_bump_gradient(t_bump bump, t_vec3 pu, t_vec3 pv,
	double (*height_fn)(t_vec2))
{
	static const double	epsilon = 1e-4;
	double				b00;
	double				bu;
	double				bv;
	t_vec3				n_prime;

	b00 = height_fn(bump.uv);
	bu = (height_fn((t_vec2){{uv.x + epsilon, uv.y}}) - b00) / epsilon;
	bv = (height_fn((t_vec2){{uv.x, uv.y + epsilon}}) - b00) / epsilon;
	n_prime = vec3_add(vec3_sub(
				bump.normal, vec3_scale(vec3_cross(bump.normal, pv), bu)),
			vec3_scale(vec3_cross(bump.normal, pu), bv));
	return (vec3_normalize(n_prime));
}

t_vec3	bump_normal_sphere(t_hit_record rec, t_vec2 uv,
	double (*height_fn)(t_vec2))
{
	t_bump	bump;
	t_vec3	pu;
	t_vec3	pv;
	t_vec3	n_prime;

	bump.cos_theta = cos(uv.y *  M_PI);
	bump.sin_theta = sin(uv.y * M_PI);
	bump.cos_phi = cos(uv.x * 2.0 *  M_PI);
	bump.sin_phi = sin(uv.x * M_PI);
	bump.normal = vec3_normalize(vec3_sub(rec.point, rec.object.center));
	bump.obj = rec.obj;
	bump.uv = uv;
	pu = vec3_scale((t_vec3){{-bum.sin_phi, 0, bump.cos_phi}},
		M_PI * rec.object->props.radius * bump.sin_theta);
	pv =  vec3_scale((t_vec3){{bump.sin_theta * bump.cos_phi,
			-bump.cos_theta, bump.cos_theta * bump.sin_phi}},
		M_PI * rec.object->props.radius);
	n_prime = get_bump_gradient(bump, pu, pv, height_fn);
	return (n_prime);
}
