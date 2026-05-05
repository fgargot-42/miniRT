/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 18:57:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 21:51:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_object *obj,
	t_hit_ctx ctx)
{
	double				z_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	z_cap = 2 * (int)(ctx.render_hit.z > 0) - 1;
	if (fabs(fabs(ctx.render_hit.z) - obj->props.height / 2.0) < 1e-3)
		rec->normal = vec3_normalize((t_vec3){0, 0, z_cap});
	else
		rec->normal = vec3_normalize(vec3_multiply(ctx.render_hit, z_scale));
	if (fabs(obj->direction.z - 1) > 1e-3)
		rec->normal = vec_reverse_rotation(rec->normal,
				obj->props.transform_axis);
	rec->t = vec3_distance(ctx.render_hit, ctx.oc);
	rec->normal = face_normal(ray, rec->normal);
	rec->point = ray_at(*ray, rec->t);
	rec->color = obj->color;
	rec->object = obj;
	rec->specular = obj->specular;
	rec->shininess = obj->shininess;
}

static int	get_intersection(double radius, double height, t_hit_ctx *ctx)
{
	double				roots[2];
	int					nb_roots;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	nb_roots = get_polynom2_roots(roots,
			vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->rd),
			2.0 * vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->oc),
			vec3_dot(vec3_multiply(ctx->oc, z_scale), ctx->oc)
			- radius * radius);
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots[0]));
	ctx->render_t = roots[0];
	if (roots[0] < T_MIN || fabs(ctx->render_hit.z) > height / 2.0)
	{
		ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots[1]));
		ctx->render_t = roots[1];
	}
	if (ctx->render_t < T_MIN || ctx->render_t > ctx->t_max)
		return (0);
	if (fabs(ctx->render_hit.z) > height / 2.0)
		return (0);
	return (1);
}

static int	hit_cylinder_cap(double radius, double height, t_hit_ctx *ctx)
{
	double				v_len;
	t_vec3				v_hit_cap;
	static const t_vec3	z_scale = (t_vec3){1, 1, 0};

	v_len = height / 2.0;
	if (ctx->oc.z < 0)
		v_len = -height / 2.0;
	if (fabs(ctx->oc.z) < height / 2.0)
	{
		if (vec3_length(vec3_multiply(ctx->oc, z_scale)) > radius + 1e-3)
			return (0);
		v_len = (2 * (ctx->rd.z > 0) - 1) * height / 2.0;
	}
	v_len = fabs((v_len - ctx->oc.z) / ctx->rd.z);
	v_hit_cap = vec3_add(ctx->oc, vec3_scale(ctx->rd, v_len));
	v_len = vec3_distance(v_hit_cap, ctx->oc);
	if (vec3_length(vec3_multiply(v_hit_cap, z_scale)) > radius + 1e-3)
		return (0);
	if (fabs(v_hit_cap.z) > height / 2.0 + 1e-3 || v_len < T_MIN
		|| v_len >= ctx->t_max || v_len > ctx->render_t)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = v_len;
	return (1);
}

int	hit_cylinder(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec)
{
	int			has_hit;
	t_hit_ctx	ctx;

	ctx.t_max = t_max;
	ctx.obj_ray = get_object_relative_ray(*ray, obj);
	has_hit = get_intersection(obj->radius, obj->props.height, &ctx);
	has_hit |= hit_cylinder_cap(obj->radius, obj->props.height, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, obj, ctx);
	return (1);
}
