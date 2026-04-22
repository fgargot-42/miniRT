/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_sphere.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 19:22:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:09:28 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static void	get_roots(double *roots, double a, double b, double c)
{
	double	delta;
	double	sqrt_d;

	delta = b * b - 4 * a * c;
	if (delta < 0)
		return ;
	sqrt_d = sqrt(delta);
	roots[0] = (-b - sqrt_d) / (2.0 * a);
	roots[1] = (-b + sqrt_d) / (2.0 * a);
}

static void	update_hit_record(t_hit_record *rec, double root, t_ray *ray,
	t_sphere *sphere)
{
	rec->t = root;
	rec->point = ray_at(*ray, rec->t);
	rec->normal = face_normal(ray, vec3_normalize(vec3_sub(
					rec->point, sphere->center)));
	rec->color = sphere->color;
	if (DEBUG && vec3_dot(ray->direction, rec->normal) > 0)
		rec->color = (t_vec3){255, 0, 255};
	rec->object.sphere = sphere;
}

int	hit_sphere(t_sphere *sphere, t_ray *ray, double t_max, t_hit_record *rec)
{
	t_vec3	oc;
	double	a;
	double	b;
	double	c;
	double	roots[2];

	oc = vec3_sub(ray->origin, sphere->center);
	a = vec3_dot(ray->direction, ray->direction);
	b = 2.0 * vec3_dot(oc, ray->direction);
	c = vec3_dot(oc, oc) - sphere->radius * sphere->radius;
	if (b * b < 4 * a * c)
		return (0);
	get_roots(roots, a, b, c);
	if (roots[0] < T_MIN || roots[0] > t_max)
	{
		if (roots[1] < T_MIN || roots[1] > t_max)
			return (0);
		update_hit_record(rec, roots[1], ray, sphere);
		return (1);
	}
	update_hit_record(rec, roots[0], ray, sphere);
	return (1);
}
