/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawer_utils_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/23 22:50:02 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/23 23:31:27 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "veclib.h"
#include "miniRT_bonus.h"
#include "hit_bonus.h"

t_vec3	draw_skybox(t_scene *scene, t_ray r)
{
	t_vec2	uv;
	t_vec3	uvcol;

	uv = get_uv(r.direction);
	uv.x = uv.x - floor(uv.x);
	uv.y = uv.y - floor(uv.y);
	uvcol = uv_to_color(scene->skybox, uv);
	return (uvcol);
}
