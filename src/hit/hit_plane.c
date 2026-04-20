/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_plane.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:05:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/20 19:49:10 by fgargot          ###   ########.fr       */
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
	if (DEBUG && vec_dot(ray->direction, rec->normal) > 0)
		rec->color = (t_vec3){255, 0, 255};
	rec->object.plane = plane;
}

int	hit_plane(t_plane *plane, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	d;
	double	t;
	int		x;
	int		z;

	d = vec_dot(ray->direction, plane->normal);
	if (fabs(d) < 1e-6)
		return (0);
	t = vec_dot(vec_sub(plane->point, ray->origin), plane->normal) / d;
	if (t < T_MIN || t > t_max)
		return (0);
	update_hit_record(rec, t, ray, plane);
	if (plane->checker)
	{
		x = (int)floor(rec->point.x);
		z = (int)floor(rec->point.z);
		if ((x + z) % 2 == 0)
			rec->color = (t_vec3){255, 255, 255};
		else
			rec->color = (t_vec3){30, 30, 30};
	}
	else
		rec->color = plane->color;
	return (1);
}
