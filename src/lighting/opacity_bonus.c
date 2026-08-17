/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   opacity_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 01:32:48 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:37:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "object_bonus.h"
#include "hit_bonus.h"
#include "material_bonus.h"
#include "uv_bonus.h"
#include <stdio.h>

double	get_object_hit_opacity(t_hit_record rec)
{
	double	opacity;
	t_vec3	col;
	t_vec2	uv;

	if (!rec.object->material)
		return (1.0);
	opacity = rec.object->material->opacity;
	if (rec.object->material->mask_tex)
	{
		uv = get_uv(rec.object, rec.point);
		col = uv_to_color(rec.object, rec.object->material->mask_tex, uv);
		opacity = col.x / 255.0;
	}
	return (opacity);
}
