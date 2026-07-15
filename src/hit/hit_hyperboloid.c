/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_hyperboloid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/09 16:34:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/15 20:46:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

// Hyperboloid equation: z² = x² + y² - 1

static double	get_hyperboloid_z_radius(t_vec3 point, double tan_angle,
	double radius)
{
	double	z_radius;

	if (fabs(tan_angle) < 1e-6)
		return (0);
	point.z = 0;
	point = vec3_scale(point, 1 / radius);
	z_radius = vec3_dot(point, point) - 1;
	if (z_radius <= 0)
		return (0);
	return (sqrt(z_radius) / tan_angle);
}

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_object *obj,
	t_hit_ctx ctx)
{
	t_vec3	normal;
	double	z_cap;

	z_cap = 2 * (ctx.render_hit.z > 0) - 1;
	normal = (t_vec3){{0, 0, z_cap}};
	if (ctx.render_hit.z < obj->props.height - 1e-6
		&& ctx.render_hit.z > -obj->props.depth + 1e-6)
	{
		normal = vec3_scale(ctx.render_hit, 1 / pow(obj->radius, 2));
		normal.z = -ctx.render_hit.z * pow(obj->props.tan_angle, 2);
	}
	if (fabs(obj->direction.z - 1) > 1e-6)
		normal = vec_reverse_rotation(normal, obj->props.transform_axis);
	rec->t = vec3_distance(ctx.render_hit, ctx.oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = vec3_normalize(normal);
	rec->color = obj->color;
	rec->object = obj;
	rec->specular = obj->specular;
	rec->shininess = obj->shininess;
}

static int	get_intersection(t_obj_prop props, double radius, t_hit_ctx *ctx)
{
	double	roots_tmp[2];
	int		nb_roots;
	t_vec3	z_scale;

	z_scale = (t_vec3){{1 / pow(radius, 2), 1 / pow(radius, 2),
		-pow(props.tan_angle, 2)}};
	nb_roots = get_polynom2_roots(roots_tmp,
			vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->rd),
			2.0 * vec3_dot(vec3_multiply(ctx->rd, z_scale), ctx->oc),
			vec3_dot(vec3_multiply(ctx->oc, z_scale), ctx->oc) - 1);
	if (nb_roots == 0)
		return (0);
	ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[0]));
	ctx->render_t = roots_tmp[0];
	if (roots_tmp[0] < T_MIN || ctx->render_hit.z < -props.depth
		|| ctx->render_hit.z > props.height)
	{
		ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots_tmp[1]));
		ctx->render_t = roots_tmp[1];
	}
	if (ctx->render_t < T_MIN || ctx->render_t >= ctx->t_max)
		return (0);
	if (ctx->render_hit.z < -props.depth || ctx->render_hit.z > props.height)
		return (0);
	return (1);
}

static int	hit_hyperboloid_cap(t_obj_prop props, double radius, t_hit_ctx *ctx)
{
	double	v_len;
	t_vec3	v_hit_cap;
	double	z_radius;

	v_len = (ctx->oc.z > 0) * props.height - (ctx->oc.z < 0) * props.depth;
	if (ctx->oc.z < props.height && ctx->oc.z > -props.depth)
	{
		z_radius = get_hyperboloid_z_radius(ctx->oc, props.tan_angle, radius);
		if (z_radius > fabs(ctx->oc.z))
			return (0);
		v_len = (ctx->rd.z > 0) * props.height - (ctx->rd.z < 0) * props.depth;
	}
	v_len = fabs((v_len - ctx->oc.z) / ctx->rd.z);
	v_hit_cap = vec3_add(ctx->oc, vec3_scale(ctx->rd, v_len));
	v_len = vec3_distance(v_hit_cap, ctx->oc);
	if (v_len < T_MIN || (fabs(v_hit_cap.z - props.height) > 1e-3
			&& fabs(v_hit_cap.z + props.depth) > 1e-3))
		return (0);
	z_radius = get_hyperboloid_z_radius(v_hit_cap, props.tan_angle, radius);
	if (z_radius > fabs(v_hit_cap.z) || v_len >= ctx->t_max)
		return (0);
	ctx->render_hit = v_hit_cap;
	ctx->render_t = v_len;
	return (1);
}

int	hit_hyperboloid(t_object *obj, t_ray *ray, double t_max, t_hit_record *rec)
{
	int				has_hit;
	t_hit_ctx		ctx;

	ctx.t_max = t_max;
	ctx.render_t = t_max;
	ctx.obj_ray = get_object_relative_ray(*ray, obj);
	has_hit = get_intersection(obj->props, obj->radius, &ctx);
	has_hit |= hit_hyperboloid_cap(obj->props, obj->radius, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, obj, ctx);
	apply_checker(rec, obj, ctx.render_hit);
	return (1);
}
