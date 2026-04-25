/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:05:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 20:19:49 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static void	update_hit_record(t_hit_record *rec, double t, t_ray *ray,
	t_plane *plane)
{
	rec->t = t;
	rec->point = ray_at(*ray, t);
	rec->normal = face_normal(ray, plane->normal);
	//if (DEBUG && vec3_dot(ray->direction, rec->normal) > 0)
	//	rec->color = (t_vec3){255, 0, 255};
	rec->object = (t_object *)plane;
}

static void	apply_checker(t_hit_record *rec)
{
	int		x;
	int		z;

	x = (int)floor(rec->point.x);
	z = (int)floor(rec->point.z);
	if ((x + z) % 2 == 0)
		rec->color = (t_vec3){255, 255, 255};
	else
		rec->color = (t_vec3){30, 30, 30};
}

int	hit_plane(void *plane, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	d;
	double	t;
	t_plane	*pl;

	pl = (t_plane *)plane;
	d = vec3_dot(ray->direction, pl->normal);
	if (fabs(d) < 1e-6)
		return (0);
	t = vec3_dot(vec3_sub(pl->point, ray->origin), pl->normal) / d;
	if (t < T_MIN || t > t_max)
		return (0);
	update_hit_record(rec, t, ray, pl);
	if (pl->checker)
		apply_checker(rec);
	else
		rec->color = pl->color;
	return (1);
}
