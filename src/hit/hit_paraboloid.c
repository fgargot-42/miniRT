/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_paraboloid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/30 23:35:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_object *obj,
	t_hit_ctx ctx)
{
	t_paraboloid	*pa;
	t_vec3			normal;
	double			z_cap;

	pa = (t_paraboloid *)obj->object;
	z_cap = 2 * (ctx.render_hit.z > 0) - 1;
	normal = (t_vec3){0, 0, z_cap};
	if (ctx.render_hit.z < pa->height - 1e-3
		&& ctx.render_hit.z > -pa->depth + 1e-3)
	{
		normal = vec3_normalize((t_vec3){ctx.render_hit.x,
				ctx.render_hit.y, 0});
		normal = vec3_add(normal, (t_vec3){0, 0, -z_cap * pa->tan_angle});
	}
	if (fabs(pa->axis.z - 1) > 1e-3)
		normal = vec_reverse_rotation(normal, pa->transform_axis);
	rec->t = vec3_distance(ctx.render_hit, ctx.oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = face_normal(ray, vec3_normalize(normal));
	rec->color = pa->color;
	rec->object = obj;
	rec->specular = pa->specular;
	rec->shininess = pa->shininess;
}

static int	get_intersection(t_paraboloid *pa, t_hit_ctx *ctx)
{
	double	roots_tmp[2];
	int		nb_roots;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};
	t_vec3	oc_no_z;
	t_vec3	rd_no_z;

	oc_no_z = vec3_multiply(ctx->oc, z_scale);
	rd_no_z = vec3_multiply(ctx->rd, z_scale);
	nb_roots = get_polynom2_roots(roots_tmp, vec3_dot(rd_no_z, rd_no_z),
			2.0 * vec3_dot(oc_no_z, rd_no_z) - ctx->rd.z / pa->tan_angle,
			vec3_dot(oc_no_z, oc_no_z) - ctx->oc.z / pa->tan_angle);
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[0]));
	ctx->render_t = roots_tmp[0];
	if (roots_tmp[0] < T_MIN || ctx->render_hit.z < -pa->depth
		|| ctx->render_hit.z > pa->height)
	{
		ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[1]));
		ctx->render_t = roots_tmp[1];
	}
	if (ctx->render_t < T_MIN || ctx->render_t >= ctx->t_max)
		return (0);
	if (ctx->render_hit.z < -pa->depth || ctx->render_hit.z > pa->height)
		return (0);
	return (1);
}

static int	hit_paraboloid_cap(t_paraboloid *pa, t_hit_ctx *ctx)
{
	double				v_len;
	t_vec3				v_hit_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	return (0);
	v_len = (ctx->oc.z > 0) * pa->height - (ctx->oc.z < 0) * pa->depth;
	if (ctx->oc.z < pa->height && ctx->oc.z > -pa->depth)
		if (vec3_length(vec3_multiply(ctx->oc, z_scale)) > fabs(ctx->oc.z
				* pa->tan_angle) + 1e-3
			|| (ctx->oc.z > 0) != (ctx->rd.z > 0))
			return (0);
	v_len = fabs((v_len - ctx->oc.z) / ctx->rd.z);
	v_hit_cap = vec3_add(ctx->oc, vec3_scale(ctx->rd, v_len));
	v_len = vec3_distance(v_hit_cap, ctx->oc);
	if (v_len < T_MIN)
		return (0);
	if (fabs(v_hit_cap.z - pa->height) > 1e-3
		&& fabs(v_hit_cap.z + pa->depth) > 1e-3)
		return (0);
	if (vec3_length(vec3_multiply(v_hit_cap, z_scale)) > fabs(v_hit_cap.z)
		* pa->tan_angle)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = v_len;
	return (1);
}

int	hit_paraboloid(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec)
{
	int				has_hit;
	t_hit_ctx		ctx;
	t_paraboloid	*pa;

	pa = (t_paraboloid *)obj->object;
	ctx.oc = vec3_sub(ray->origin, pa->center);
	ctx.rd = ray->direction;
	ctx.t_max = t_max;
	ctx.render_t = t_max;
	if (fabs(pa->axis.z - 1) > 1e-3)
	{
		ctx.oc = vec_apply_rotation_z(ctx.oc, pa->transform_axis);
		ctx.rd = vec_apply_rotation_z(ctx.rd, pa->transform_axis);
	}
	has_hit = get_intersection(pa, &ctx);
	has_hit |= hit_paraboloid_cap(pa, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, obj, ctx);
	return (1);
}
