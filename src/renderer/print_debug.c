/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 18:32:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/27 21:51:33 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	print_hit_info(t_hit_record hc, double mouse_x, double mouse_y)
{
	printf("\nObject selected at address %p\n", hc.object);
	if (!hc.object)
		return ;
	printf("Object type: %d\n", hc.object->type);
	printf("Object hit coordinates: x=%.3f y=%.3f z=%.3f (u=%.3f, v=%.3f)\n",
		hc.point.x, hc.point.y, hc.point.z, mouse_x, mouse_y);		
	printf("Object normal at hit: x=%.3f y=%.3f z=%.3f\n", hc.normal.x,
		hc.normal.y, hc.normal.z);
	printf("Object color at hit: x=%.3f y=%.3f z=%.3f\n", hc.color.x,
		hc.color.y, hc.color.z);
}
