/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_bonus.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 01:30:21 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:36:40 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UV_BONUS_H
# define UV_BONUS_H

# include "object_bonus.h"
# include "veclib.h"

t_vec2	get_uv(t_object *obj, t_vec3 vec);
t_vec2	get_sphere_uv(t_object *obj, t_vec3 point);
t_vec2	get_triangle_uv(t_object *obj, t_vec3 point);
t_vec3	uv_to_color(t_object *obj, t_texture *tex, t_vec2 uv);

#endif // UV_BONUS_H
