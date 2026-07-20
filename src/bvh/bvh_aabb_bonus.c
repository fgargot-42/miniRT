/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_aabb_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:23:31 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:06:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "bvh_bonus.h"

void	get_object_aabb(t_object *obj, t_aabb *aabb)
{
	static t_obj_aabb_fn	aabb_fn[] = {
	[OBJ_SPHERE] = get_sphere_aabb,
	[OBJ_CYLINDER] = get_cylinder_aabb,
	[OBJ_CONE] = get_cone_aabb,
	[OBJ_HYPERBOLOID] = get_hyperboloid_aabb,
	[OBJ_PARABOLOID] = get_paraboloid_aabb,
	[OBJ_TRIANGLE] = get_triangle_aabb
	};

	if (obj->type >= OBJ_SPHERE && obj->type <= OBJ_TRIANGLE)
		aabb_fn[obj->type](obj, aabb);
}
