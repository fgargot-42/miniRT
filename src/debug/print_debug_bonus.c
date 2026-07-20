/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:40:59 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 22:46:00 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "debug_bonus.h"

void	print_object(void *o)
{
	t_object	*obj;

	obj = (t_object *)o;
	if (obj->type == OBJ_SPHERE)
		print_sphere(obj);
	if (obj->type == OBJ_PLANE)
		print_plane(obj);
	if (obj->type == OBJ_CYLINDER)
		print_cylinder(obj);
	if (obj->type == OBJ_CONE)
		print_cone(obj);
	if (obj->type == OBJ_HYPERBOLOID)
		print_hyper(obj);
	if (obj->type == OBJ_PARABOLOID)
		print_para(obj);
	if (obj->type == OBJ_TRIANGLE)
		print_triangle(obj);
	if (obj->type == OBJ_LIGHT)
		print_light(obj);
}
