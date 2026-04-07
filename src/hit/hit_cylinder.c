/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_cylinder.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/02 18:57:53 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/07 21:29:09 by fgargot          ###   ########.fr       */
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

static int	get_polynom2_roots(double *roots, double a, double b, double c)
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

static void	update_hit_record(t_hit_record *rec, t_ray *ray, t_cylinder *cyl,
	t_vec3 v_hit)
{
	t_vec3	normal;
	t_vec3	oc;

	oc = vec_apply_rotation_z(vec_sub(ray->origin, cyl->center),
		cyl->transform_axis);
	//double	cap_z;

	//cap_z = (2 * (v_hit.z >= 0) - 1) * cyl->height;
//	if (fabs(v_hit.z) == cyl->height / 2.0)
//		normal = vec_reverse_rotation(
//				(t_vec3){0, 0, 2 * (cap_z > 0) - 1}, cyl->transform_axis);
//	else
		normal = vec_normalize(vec_reverse_rotation(
				(t_vec3){v_hit.x, v_hit.y, 0}, cyl->transform_axis));
	rec->t = vec_distance(v_hit, oc);
	rec->point = ray_at(*ray, rec->t);
	rec->normal = face_normal(ray, normal);
	rec->color = cyl->color;
	rec->object.cylinder = cyl;
}

static int	get_intersections(double *roots, t_vec3 *v_hit, t_ray *ray,
	t_cylinder *cyl)
{
	t_vec3	oc;
	t_vec3	rd;
	int		nb_roots;

	oc = vec_sub(ray->origin, cyl->center);
	oc = vec_apply_rotation_z(oc, cyl->transform_axis);
	rd = vec_apply_rotation_z(ray->direction, cyl->transform_axis);
	nb_roots = get_polynom2_roots(roots,
			rd.x * rd.x + rd.y * rd.y,
			2.0 * (oc.x * rd.x + oc.y * rd.y),
			(oc.x * oc.x + oc.y * oc.y) - cyl->radius * cyl->radius);
	if (nb_roots == 0)
		return (0);
	v_hit[0] = vec_add(oc, vec_scale(rd, roots[0]));
	v_hit[1] = vec_add(oc, vec_scale(rd, roots[1]));
	return (1);
}

int	hit_cylinder(t_cylinder *cyl, t_ray *ray, double t_max, t_hit_record *rec)
{
	double	roots[2];
	t_vec3	v_hit[2];
	int		has_intersections;

	has_intersections = get_intersections(roots, v_hit, ray, cyl);
	if (!has_intersections)
		return (0);
	if (roots[0] >= T_MIN && roots[0] <= t_max
		&& fabs(v_hit[0].z) <= cyl->height / 2.0)
	{
		update_hit_record(rec, ray, cyl, v_hit[0]);
		return (1);
	}
	if (fabs(v_hit[1].z) > (cyl->height / 2.0))// && (v_hit[0].z > 0) == (v_hit[1].z > 0))
		return (0);
	//roots[1] = (roots[1] - roots[0]) * ((cap_z - v_hit[0].z) / (v_hit[1].z - v_hit[0].z)) + roots[0];
	if (roots[1] >= T_MIN && roots[1] <= t_max)
	{
		update_hit_record(rec, ray, cyl, v_hit[1]);
		return (1);
	}
	return (0);
}
