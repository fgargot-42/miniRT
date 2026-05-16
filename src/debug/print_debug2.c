/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:49:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/16 20:07:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "miniRT.h"

void	print_cylinder(t_object *c)
{
	t_vec3	b_min;
	t_vec3	b_max;

	get_cylinder_aabb(c, &b_min, &b_max);
	printf("Object type: CYLINDER\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f h=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->radius,
		c->props.height);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b_min.x, b_min.y, b_min.z, b_max.x, b_max.y, b_max.z);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x,
		c->color.y, c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_cone(t_object *c)
{
	t_vec3	b_min;
	t_vec3	b_max;

	get_cone_aabb(c, &b_min, &b_max);
	printf("Object type: CONE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle,
		c->props.height, c->props.depth);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b_min.x, b_min.y, b_min.z, b_max.x, b_max.y, b_max.z);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_hyper(t_object *c)
{
	t_vec3	b_min;
	t_vec3	b_max;

	get_hyperboloid_aabb(c, &b_min, &b_max);
	printf("Object type: HYPERBOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle,
		c->props.height, c->props.depth);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b_min.x, b_min.y, b_min.z, b_max.x, b_max.y, b_max.z);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_para(t_object *c)
{
	t_vec3	b_min;
	t_vec3	b_max;

	get_paraboloid_aabb(c, &b_min, &b_max);
	printf("Object type: PARABOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f)\n",
		c->position.x, c->position.y, c->position.z, c->angle, c->props.height);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b_min.x, b_min.y, b_min.z, b_max.x, b_max.y, b_max.z);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->direction.x, c->direction.y, c->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", c->color.x, c->color.y,
		c->color.z);
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}

void	print_triangle(t_object *c)
{
	t_vec3	b_min;
	t_vec3	b_max;

	get_triangle_aabb(c, &b_min, &b_max);
	printf("Object type: TRIANGLE\n");
	printf("--a: x=%.2f y=%.2f z=%.2f\n",
		c->props.a.x, c->props.a.y, c->props.a.z);
	printf("--b: x=%.2f y=%.2f z=%.2f\n",
		c->props.b.x, c->props.b.y, c->props.b.z);
	printf("--c: x=%.2f y=%.2f z=%.2f\n",
		c->props.c.x, c->props.c.y, c->props.c.z);
	printf("--bounding box: [ %.2f %.2f %.2f ] [ %.2f %.2f, %.2f ]\n",
		b_min.x, b_min.y, b_min.z, b_max.x, b_max.y, b_max.z);
	printf("--color: x=%.2f y=%.2f z=%.2f (checkered: ", c->color.x, c->color.y,
		c->color.z);
	if (c->checker)
		printf("true)\n");
	else
		printf("false)\n");
	printf("--specular: %.2f\n", c->specular);
	printf("--shininess: %.2f\n", c->shininess);
}
