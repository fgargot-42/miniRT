/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:49:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/06 23:21:15 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "miniRT.h"

void	print_cylinder(t_object *c)
{
	printf("Object type: CYLINDER\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f h=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->radius,
		c->props.height);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x,
		c->color.y, c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_cone(t_object *c)
{
	printf("Object type: CONE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle,
		c->props.height, c->props.depth);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_hyper(t_object *c)
{
	printf("Object type: HYPERBOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle,
		c->props.height, c->props.depth);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_para(t_object *c)
{
	printf("Object type: PARABOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle, c->props.height);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_triangle(t_object *c)
{
	printf("Object type: TRIANGLE\n");
	printf("--a: x=%.2f y=%.2f z=%.2f\n",
		c->props.a.x, c->props.a.y, c->props.a.z);
	printf("--b: x=%.2f y=%.2f z=%.2f\n",
		c->props.b.x, c->props.b.y, c->props.b.z);
	printf("--c: x=%.2f y=%.2f z=%.2f\n",
		c->props.c.x, c->props.c.y, c->props.c.z);
	printf("--color: x=%.2f y=%.2f z=%.2f (checkered: ", c->color.x, c->color.y,
		c->color.z);
	if (c->checker)
		printf("true)\n");
	else
		printf("false)\n");
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}
