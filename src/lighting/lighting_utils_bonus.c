/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting_utils_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 19:44:24 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/23 00:47:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hit_bonus.h"
#include "miniRT_bonus.h"
#include "normal.h"
#include "veclib.h"

void apply_normal(t_ray *ray, t_hit_record *rec)
{
	t_vec2	uv;

	rec->normal = face_normal(ray, rec->normal);
	if (!rec->object->material || !rec->object->material->normal_tex)
		return ;
	if (rec->object->type == OBJ_TRIANGLE)
	{
		uv = get_triangle_uv_hit(rec->object, rec->point);
		rec->normal = bump_normal_triangle(*rec, uv, get_bump_from_img);
	}
	if (rec->object->type == OBJ_SPHERE)
	{
		uv = get_uv(rec->normal);
		rec->normal = bump_normal_sphere(*rec, uv, get_bump_from_img);
	}
}
