/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   veclib3_math3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 23:56:24 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/02 20:49:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include <math.h>

t_vec3	vec3_min(t_vec3 a, t_vec3 b)
{
	if (b.x < a.x)
		a.x = b.x;
	if (b.y < a.y)
		a.y = b.y;
	if (b.z < a.z)
		a.z = b.z;
	return (a);
}

t_vec3	vec3_max(t_vec3 a, t_vec3 b)
{
	if (b.x > a.x)
		a.x = b.x;
	if (b.y > a.y)
		a.y = b.y;
	if (b.z > a.z)
		a.z = b.z;
	return (a);
}

t_vec3	vec3_pow(t_vec3 a, double p)
{
	t_vec3	res;
	
	res.x = pow(a.x, p);
	res.y = pow(a.y, p);
	res.z = pow(a.z, p);
	return (res);
}
