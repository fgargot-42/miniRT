/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 18:25:17 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/09 21:41:55 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include <math.h>

#include <stdio.h>

t_vec3	srgb_to_linear(t_vec3 srgb)
{
	t_vec3	linear_rgb;

	if (srgb.x <= 0.04045)
		linear_rgb.x = srgb.x / 12.92;
	else
		linear_rgb.x = pow((srgb.x + 0.055) / 1.055, 2.4);
	if (srgb.y <= 0.04045)
		linear_rgb.y = srgb.y / 12.92;
	else
		linear_rgb.y = pow((srgb.y + 0.055) / 1.055, 2.4);
	if (srgb.z <= 0.04045)
		linear_rgb.z = srgb.z / 12.92;
	else
		linear_rgb.z = pow((srgb.z + 0.055) / 1.055, 2.4);
	linear_rgb = vec3_scale(linear_rgb, 255);
	return (linear_rgb);
}

t_vec3	linear_to_srgb(t_vec3 linear_rgb)
{
	t_vec3	srgb;

	if (linear_rgb.x <= 0.0031308)
		srgb.x = linear_rgb.x * 12.92;
	else
		srgb.x = 1.055 * pow((linear_rgb.x), 1 / 2.4) - 0.055;
	if (linear_rgb.y <= 0.0031308)
		srgb.y = linear_rgb.y * 12.92;
	else
		srgb.y = 1.055 * pow((linear_rgb.y), 1 / 2.4) - 0.055;
	if (linear_rgb.z <= 0.0031308)
		srgb.z = linear_rgb.z * 12.92;
	else
		srgb.z = 1.055 * pow((linear_rgb.z), 1 / 2.4) - 0.055;
	srgb = vec3_scale(srgb, 255);
	return (srgb);
}
