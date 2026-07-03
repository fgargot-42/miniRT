/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/03 17:56:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/03 18:00:41 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>

double smoothstep(double min, double max, double value)
{
	double	res;

	if (min == max)
		return (1.0);
	res = fmax(0.0, fmin(1.0, (value - min) / (max - min)));
	return (res * res * (3.0 - 2.0 * res));
}
