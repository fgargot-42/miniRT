/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_object_debug.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:47:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 00:34:14 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdio.h>

void	print_cylinder(t_object *c)
{
	t_aabb	b;

	get_cylinder_aabb(c, &b);
	printf("Object type: CYLINDER\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f h=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->radius,
		c->props.height);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b.min.x, b.min.y, b.min.z, b.max.x, b.max.y, b.max.z);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x,
		c->color.y, c->color.z);
}

void	print_light(t_object *s)
{
	printf("Object type: LIGHT\n");
	printf("--position: x=%.2f y=%.2f z=%.2f\n",
		s->position.x, s->position.y, s->position.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n",
		s->color.x, s->color.y, s->color.z);
	printf("--intensity: %.2f\n", s->props.intensity);
}

void	print_sphere(t_object *s)
{
	t_aabb	b;

	get_sphere_aabb(s, &b);
	printf("Object type: SPHERE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f)\n",
		s->position.x, s->position.y, s->position.z, s->radius);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f %.2f ]\n",
		b.min.x, b.min.y, b.min.z,
		b.max.x, b.max.y, b.max.z);
}

void	print_plane(t_object *p)
{
	printf("Object type: PLANE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f\n",
		p->position.x, p->position.y, p->position.z);
	printf("--normal: x=%.2f y=%.2f z=%.2f\n",
		p->direction.x, p->direction.y, p->direction.z);
}
