/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:23:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:06:29 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "bvh.h"

void	get_object_aabb(t_object *obj, t_aabb *aabb)
{
	if (obj->type == OBJ_SPHERE)
		get_sphere_aabb(obj, aabb);
	if (obj->type == OBJ_CYLINDER)
		get_cylinder_aabb(obj, aabb);
}
