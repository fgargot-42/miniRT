/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/27 19:06:29 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cone *cone,
	t_hit_ctx ctx)
{
	t_vec3	normal;
	double	z_cap;

	z_cap = 2 * (ctx.render_hit.z > 0) - 1;
	normal = (t_vec3){ctx.render_hit.x, ctx.render_hit.y,
		-z_cap * cone->tan_angle};
	if (fabs(ctx.render_hit.z - cone->height) < 1e-9
		|| fabs(ctx.render_hit.z + cone->depth) < 1e-9)
		normal = (t_vec3){0, 0, z_cap};
	if (fabs(cone->axis.z - 1) > 1e-9)
		normal = vec_reverse_rotation(normal, cone->transform_axis);
	rec->t = vec3_distance(ctx.render_hit, ctx.oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = face_normal(ray, vec3_normalize(normal));
	rec->color = cone->color;
	//if (DEBUG && vec3_dot(ray->direction, rec->normal) > 1)
	//	rec->color = (t_vec3){255, 0, 255};
	rec->object = (t_object *)cone;
	rec->specular = cone->specular;
	rec->shininess = cone->shininess;
}

static int	get_intersection(t_cone *cone, t_hit_ctx *ctx)
{
	double	roots_tmp[2];
	int		nb_roots;
	t_vec3	z_scale;

	z_scale = (t_vec3){1, 1, -cone->tan_angle * cone->tan_angle};
	nb_roots = get_polynom2_roots(roots_tmp,
			vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->rd),
			2.0 * vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->oc),
			vec3_dot(vec3_multiply(ctx->oc, z_scale), ctx->oc));
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[0]));
	ctx->render_t = roots_tmp[0];
	if (roots_tmp[0] < T_MIN || ctx->render_hit.z < -cone->depth
		|| ctx->render_hit.z > cone->height)
	{
		ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[1]));
		ctx->render_t = roots_tmp[1];
	}
	if (ctx->render_t < T_MIN || ctx->render_t >= ctx->t_max)
		return (0);
	if (ctx->render_hit.z < -cone->depth || ctx->render_hit.z > cone->height)
		return (0);
	return (1);
}

static int	hit_cone_cap(t_cone *cone, t_hit_ctx *ctx)
{
	double				v_len;
	t_vec3				v_hit_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	v_len = (ctx->oc.z > 0) * cone->height - (ctx->oc.z < 0) * cone->depth;
	if (ctx->oc.z < cone->height && ctx->oc.z > -cone->depth)
		if (vec3_length(vec3_multiply(ctx->oc, z_scale)) > fabs(ctx->oc.z
				* cone->tan_angle) || (ctx->oc.z > 0) != (ctx->rd.z > 0))
			return (0);
	v_len = fabs((v_len - ctx->oc.z) / ctx->rd.z);
	v_hit_cap = vec3_add(ctx->oc, vec3_scale(ctx->rd, v_len));
	v_len = vec3_distance(v_hit_cap, ctx->oc);
	if (v_len < T_MIN || v_len > ctx->t_max || v_len > ctx->render_t)
		return (0);
	if (fabs(v_hit_cap.z - cone->height) > 1e-9
		&& fabs(v_hit_cap.z + cone->depth) > 1e-9)
		return (0);
	if (vec3_length(vec3_multiply(v_hit_cap, z_scale)) > fabs(v_hit_cap.z)
		* cone->tan_angle)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = v_len;
	return (1);
}

int	hit_cone(void *cone, t_ray *ray, double t_max, t_hit_record *rec)
{
	int			has_hit;
	t_hit_ctx	ctx;
	t_cone		*co;

	co = (t_cone *)cone;
	ctx.oc = vec3_sub(ray->origin, co->center);
	ctx.rd = ray->direction;
	ctx.t_max = t_max;
	ctx.render_t = t_max;
	if (fabs(co->axis.z - 1) > 1e-9)
	{
		ctx.oc = vec_apply_rotation_z(ctx.oc, co->transform_axis);
		ctx.rd = vec_apply_rotation_z(ctx.rd, co->transform_axis);
	}
	has_hit = get_intersection(co, &ctx);
	has_hit |= hit_cone_cap(co, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, co, ctx);
	return (1);
}
