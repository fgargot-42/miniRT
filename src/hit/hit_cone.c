/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cone.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/20 19:49:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cone *cone,
	t_vec3 v_hit)
{
	t_vec3	normal;
	t_vec3	oc;
	double	z_cap;

	z_cap = 2 * (v_hit.z > 0) - 1;
	oc = vec_apply_rotation_z(vec_sub(ray->origin, cone->center),
			cone->transform_axis);
	normal = vec_normalize(vec_reverse_rotation(
				(t_vec3){v_hit.x, v_hit.y, -z_cap * cone->tan_angle},
				cone->transform_axis));
	if (near_equal(v_hit.z, cone->height, 1e-6)
		|| near_equal(v_hit.z, -cone->depth, 1e-6))
		normal = vec_reverse_rotation((t_vec3){0, 0, z_cap},
				cone->transform_axis);
	rec->t = vec_distance(v_hit, oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = normal;
	rec->color = cone->color;
	if (DEBUG && vec_dot(ray->direction, rec->normal) > 0)
		rec->color = (t_vec3){255, 0, 255};
	rec->object.cone = cone;
}

static int	get_intersection(t_cone *cone, t_hit_ctx *ctx)
{
	double	roots_tmp[2];
	t_vec3	z_scale;
	int		nb_roots;

	z_scale = (t_vec3){1, 1, -cone->tan_angle * cone->tan_angle};
	nb_roots = get_polynom2_roots(roots_tmp,
			vec_dot(vec_multiply(ctx->rd, z_scale), ctx->rd),
			2.0 * vec_dot(vec_multiply(ctx->rd, z_scale), ctx->oc),
			vec_dot(vec_multiply(ctx->oc, z_scale), ctx->oc));
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec_add(ctx->oc, vec_scale(ctx->rd, roots_tmp[0]));
	ctx->render_t = roots_tmp[0];
	if (roots_tmp[0] < T_MIN)
	{
		ctx->render_hit = vec_add(ctx->oc, vec_scale(ctx->rd, roots_tmp[1]));
		ctx->render_t = roots_tmp[1];
	}
	if (ctx->render_hit.z < -cone->depth || ctx->render_hit.z > cone->height)
		return (0);
	return (1);
}

static int	get_nearest_cap_distance(t_cone *cone, t_hit_ctx *ctx)
{
	double	t;
	double	v_len;
	t_vec3	v_hit_cap;

	t = -1;
	if (!ctx->rd.z)
		return (-1.0);
	v_len = fmin(fabs(cone->height - ctx->oc.z), fabs(-cone->depth - ctx->oc.z));
	v_len *= fabs(ctx->rd.z);
	v_hit_cap = vec_add(ctx->oc, vec_scale(ctx->rd, v_len));
	t = vec_length(vec_sub(v_hit_cap, ctx->oc));
	if (t < T_MIN || t > ctx->t_max)
		return (0); 
	if (vec_length(vec_multiply(v_hit_cap, (t_vec3){1, 1, 0})) > v_hit_cap.z * cone->tan_angle)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = t;
	return (1);
}

static int	hit_cone_cap(t_cone *cone, t_hit_ctx *ctx)
{
	double	t;
	double	v_len;
	t_vec3	v_hit_tmp;

	t = get_nearest_cap_distance(cone, ctx);
	if (t < T_MIN || t > ctx->t_max || t > ctx->render_t)
		return (0);
	v_hit_tmp = vec_add(ctx->oc, vec_scale(ctx->rd, t));
	v_len = vec_length(vec_multiply(v_hit_tmp, (t_vec3){1, 1, 0}));
	if (!near_equal(v_hit_tmp.z, cone->height, 1e-6)
		&& !near_equal(v_hit_tmp.z, -cone->depth, 1e-6))
		return (0);
	if (v_len > cone->height * cone->tan_angle)
		return (0);
	if (v_len > cone->depth * cone->tan_angle)
		return (0);
	ctx->render_hit = v_hit_tmp;
	ctx->render_t = t;
	return (1);
}

int	hit_cone(t_cone *cone, t_ray *ray, double t_max, t_hit_record *rec)
{
	int			has_hit;
	t_hit_ctx	ctx;

	ctx.oc = vec_sub(ray->origin, cone->center);
	ctx.rd = ray->direction;
	ctx.t_max = t_max;
	if (!near_equal(cone->axis.z, 1, 1e-6))
	{
		ctx.oc = vec_apply_rotation_z(ctx.oc, cone->transform_axis);
		ctx.rd = vec_apply_rotation_z(ctx.rd, cone->transform_axis);
	}
	has_hit = get_intersection(cone, &ctx);
	has_hit |= hit_cone_cap(cone, &ctx);
	if (!has_hit || ctx.render_t < T_MIN || ctx.render_t > t_max)
		return (0);
	update_hit_record(rec, ray, cone, ctx.render_hit);
	return (1);
}
