/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:22:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 20:29:33 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_sphere *sphere,
	t_hit_ctx ctx)
{
	rec->t = ctx.render_t;
	rec->point = ray_at(*ray, rec->t);
	rec->normal = face_normal(ray, vec3_normalize(vec3_sub(
					rec->point, sphere->center)));
	rec->color = sphere->color;
	//if (DEBUG && vec3_dot(ray->direction, rec->normal) > 0)
	//	rec->color = (t_vec3){255, 0, 255};
	rec->object = (t_object *)sphere;
}

static int	get_intersection(t_sphere *sp, t_hit_ctx *ctx)
{
	double	roots[2];
	int		nb_roots;
	double	render_hit_tmp;

	nb_roots = get_polynom2_roots(roots,
		vec3_dot(ctx->rd, ctx->rd),
		2.0 * vec3_dot(ctx->oc, ctx->rd),
		vec3_dot(ctx->oc, ctx->oc) - sp->radius * sp->radius);
	if (nb_roots == 0)
		return (0);
	render_hit_tmp = roots[0]; 
	if (roots[0] < T_MIN || roots[0] > ctx->t_max)
	{
		if (nb_roots == 1 || roots[1] < T_MIN || roots[1] > ctx->t_max)
			return (0);
		render_hit_tmp = roots[1];
	}
	ctx->render_t = render_hit_tmp;
	ctx->render_hit = vec3_add(ctx->oc, vec3_scale(ctx->rd, roots[1]));
	return (1);
}

int	hit_sphere(void *sphere, t_ray *ray, double t_max, t_hit_record *rec)
{
	int			has_hit;
	t_hit_ctx	ctx;
	t_sphere	*sp;

	sp = (t_sphere *)sphere;
	ctx.oc = vec3_sub(ray->origin, sp->center);
	ctx.rd = ray->direction;
	ctx.t_max = t_max;
	has_hit = get_intersection(sp, &ctx);
	if (!has_hit)
		return (0);
	update_hit_record(rec, ray, sp, ctx);
	return (1);
}
