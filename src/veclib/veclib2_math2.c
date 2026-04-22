/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veclib2_math2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 18:41:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 19:07:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "math.h"

t_vec2	vec2_normalize(t_vec2 v)
{
	double	len;

	len = vec2_length(v);
	if (len == 0)
		return ((t_vec2){0, 0});
	return ((t_vec2){v.x / len, v.y / len});
}

// Component-wise multiplication (for colors)
t_vec2	vec2_multiply(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){a.x * b.x, a.y * b.y});
}

t_vec2	vec2_clamp(t_vec2 v, double min, double max)
{
	return ((t_vec2){
		fmax(min, fmin(max, v.x)),
		fmax(min, fmin(max, v.y))
	});
}

double	vec2_distance(t_vec2 a, t_vec2 b)
{
	return (vec2_length(vec2_sub(b, a)));
}
