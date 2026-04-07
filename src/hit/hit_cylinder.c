/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 18:57:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/07 18:15:18 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

static t_vec3	vec_reverse_rotation(t_vec3 v_from, double **t_matrix)
{
	t_vec3	v_to;

	v_to.x = v_from.x * t_matrix[0][0] + v_from.y * t_matrix[1][0]
		+ v_from.z * t_matrix[2][0];
	v_to.y = v_from.x * t_matrix[0][1] + v_from.y * t_matrix[1][1]
		+ v_from.z * t_matrix[2][1];
	v_to.z = v_from.x * t_matrix[0][2] + v_from.y * t_matrix[1][2]
		+ v_from.z * t_matrix[2][2];
	return (v_to);
}

static int	get_polynom2_root(double *roots, double a, double b, double c)
{
	double	delta;
	double	sqrt_delta;

	delta = b * b - 4 * a * c;
	if (delta < 0)
		return (0);
	sqrt_delta = sqrt(delta);
	roots[0] = (-b - sqrt_delta) / (2.0 * a);
	roots[1] = (-b + sqrt_delta) / (2.0 * a);
	if (sqrt_delta == 0)
		return (1);
	return (2);
}

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cylinder *cyl, double root)
{
	rec->t = root;
	rec->point = ray_at(*ray, rec->t);
	rec->color = cyl->color;
	rec->object.cylinder = cyl;
}

int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_min, double t_max, t_hit_record *rec)
{
	t_vec3	oc;
	t_vec3	rd;
	t_vec3	v_hit[2];
	int		nb_roots;
	double	roots[2];

	oc = vec_sub(ray->origin, cyl->center);
	oc = vec_apply_transform(oc, cyl->transform_axis);
	rd = vec_apply_transform(ray->direction, cyl->transform_axis);
	nb_roots = get_polynom2_root(roots,
				rd.x * rd.x + rd.y * rd.y,
				2.0 * (oc.x * rd.x + oc.y * rd.y),
				(oc.x * oc.x + oc.y * oc.y) - cyl->radius * cyl->radius);
	if (nb_roots == 0)
		return (0);
	v_hit[0] = vec_add(oc, vec_scale(rd, roots[0]));
	v_hit[1] = vec_add(oc, vec_scale(rd, roots[1]));
	if (roots[0] >= t_min && roots[0] <= t_max
			&& fabs(v_hit[0].z) <= cyl->height / 2.0)
	{
		update_hit_record(rec, ray, cyl, roots[0]);
		rec->normal = vec_normalize(vec_reverse_rotation(
			(t_vec3){v_hit[0].x, v_hit[0].y, 0}, cyl->transform_axis));
		return (1);
	}
	if (fabs(v_hit[1].z) > (cyl->height / 2.0))// && (v_hit[0].z > 0) == (v_hit[1].z > 0))
		return (0);
	double cap_z = (2 * (v_hit[0].z >= 0) - 1) * cyl->height;
	//roots[1] = (roots[1] - roots[0]) * ((cap_z - v_hit[0].z) / (v_hit[1].z - v_hit[0].z)) + roots[0];
	if (roots[1] >= t_min && roots[1] <= t_max)
	{
		update_hit_record(rec, ray, cyl, roots[1]);
		rec->normal = vec_reverse_rotation((t_vec3){0, 0, 2 * (cap_z > 0) - 1}, cyl->transform_axis);
		return (1);
	}
	return (0);
}
