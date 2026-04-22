/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veclib3_math2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:41:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:07:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "math.h"

t_vec3	vec3_cross(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	});
}

t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len == 0)
		return ((t_vec3){0, 0, 0});
	return ((t_vec3){v.x / len, v.y / len, v.z / len});
}

// Component-wise multiplication (for colors)
t_vec3	vec3_multiply(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x * b.x, a.y * b.y, a.z * b.z});
}

t_vec3	vec3_clamp(t_vec3 v, double min, double max)
{
	return ((t_vec3){
		fmax(min, fmin(max, v.x)),
		fmax(min, fmin(max, v.y)),
		fmax(min, fmin(max, v.z))
	});
}

double	vec3_distance(t_vec3 a, t_vec3 b)
{
	return (vec3_length(vec3_sub(b, a)));
}
