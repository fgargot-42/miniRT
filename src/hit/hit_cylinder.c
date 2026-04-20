/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 18:57:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/20 19:57:32 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cylinder *cyl,
	t_hit_ctx ctx)
{
	double				z_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	z_cap = 2 * (ctx.render_hit.z > 0) - 1;
	if (fabs(fabs(ctx.render_hit.z) - cyl->height / 2.0) < 1e-6)
		rec->normal = vec_normalize((t_vec3){0, 0, z_cap});
	else
		rec->normal = vec_normalize(vec_multiply(ctx.render_hit, z_scale));
	if (fabs(cyl->axis.z - 1) > 1e-6)
		rec->normal = vec_reverse_rotation(rec->normal, cyl->transform_axis);
	rec->t = vec_distance(ctx.render_hit, ctx.oc);
	rec->normal = face_normal(ray, rec->normal);
	rec->point = ray_at(*ray, rec->t);
	rec->color = cyl->color;
	if (DEBUG && vec_dot(ray->direction, rec->normal) > 0)
		rec->color = (t_vec3){255, 0, 255};
	rec->object.cylinder = cyl;
}

static int	get_intersection(t_cylinder *cyl, t_hit_ctx *ctx)
{
	double				roots[2];
	int					nb_roots;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	nb_roots = get_polynom2_roots(roots,
			vec_dot(vec_multiply(ctx->rd, z_scale), ctx->rd),
			2.0 * vec_dot(vec_multiply(ctx->rd, z_scale), ctx->oc),
			vec_dot(vec_multiply(ctx->oc, z_scale), ctx->oc)
			- cyl->radius * cyl->radius);
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec_add(ctx->oc, vec_scale(ctx->rd, roots[0]));
	ctx->render_t = roots[0];
	if (roots[0] < T_MIN || fabs(ctx->render_hit.z) > cyl->height / 2.0)
	{
		ctx->render_hit = vec_add(ctx->oc, vec_scale(ctx->rd, roots[1]));
		ctx->render_t = roots[1];
	}
	if (ctx->render_t < T_MIN || ctx->render_t >= ctx->t_max)
		return (0);
	if (fabs(ctx->render_hit.z) > cyl->height / 2.0)
		return (0);
	return (1);
}

static int	hit_cylinder_cap(t_cylinder *cyl, t_hit_ctx *ctx)
{
	double				v_len;
	t_vec3				v_hit_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	v_len = (2 * (ctx->oc.z > 0) - 1 ) * cyl->height / 2.0;
	if (fabs(ctx->oc.z) < cyl->height / 2.0)
	{
		if (vec_length(vec_multiply(ctx->oc, z_scale)) > cyl->radius)
			return (0);
		v_len = (2 * (ctx->rd.z > 0) - 1 ) * cyl->height / 2.0;
	}
	v_len = fabs((v_len - ctx->oc.z) / ctx->rd.z);
	v_hit_cap = vec_add(ctx->oc, vec_scale(ctx->rd, v_len));
	v_len = vec_distance(v_hit_cap, ctx->oc);
	if (vec_length(vec_multiply(v_hit_cap, z_scale)) > cyl->radius)
		return (0);
	if (fabs(fabs(v_hit_cap.z) - cyl->height / 2.0) > 1e-6 || v_len < T_MIN
		|| v_len >= ctx->t_max || v_len > ctx->render_t)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = v_len;
	return (1);
}

int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_max, t_hit_record *rec)
{
	int			has_hit;
	t_hit_ctx	ctx;

	ctx.oc = vec_sub(ray->origin, cyl->center);
	ctx.rd = ray->direction;
	ctx.t_max = t_max;
	if (fabs(cyl->axis.z - 1) > 1e-6)
	{
		ctx.oc = vec_apply_rotation_z(ctx.oc, cyl->transform_axis);
		ctx.rd = vec_apply_rotation_z(ctx.rd, cyl->transform_axis);
	}
	has_hit = get_intersection(cyl, &ctx);
	has_hit |= hit_cylinder_cap(cyl, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, cyl, ctx);
	return (1);
}
