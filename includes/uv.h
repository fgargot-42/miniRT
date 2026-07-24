/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/25 01:30:21 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/25 01:41:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UV_H
# define UV_H

# include "object_bonus.h"
# include "veclib.h"

t_vec2	get_uv(t_object *obj, t_vec3 vec);
t_vec2	get_sphere_uv(t_object *obj, t_vec3 point);
t_vec2	get_triangle_uv(t_object *obj, t_vec3 point);
t_vec3	uv_to_color(t_texture *tex, t_vec2 uv);
t_vec3	triangle_uv_to_color(t_object *obj, t_texture *tex, t_vec3 hit);

#endif // UV_H
