/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   polynom.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 17:21:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/14 17:21:22 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

int	get_polynom2_roots(double *roots, double a, double b, double c)
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

