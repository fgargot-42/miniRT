/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:40:59 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 22:46:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "debug.h"

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
	if (obj->type == OBJ_LIGHT)
		print_light(obj);
}
