/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 22:47:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 23:05:06 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "miniRT.h"

void	print_cylinder(t_object *c);
void	print_cone(t_object *c);
void	print_hyper(t_object *c);
void	print_para(t_object *c);

static void	print_light(t_object *s)
{
	printf("Object type: LIGHT\n");
	printf("--position: x=%.2f y=%.2f z=%.2f\n",
		s->position.x, s->position.y, s->position.z);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", s->color.x, s->color.y,
		s->color.z);
	printf("--intensity: %.2f\n", s->props.intensity);
}

static void	print_sphere(t_object *s)
{
	printf("Object type: SPHERE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f)\n",
		s->position.x, s->position.y, s->position.z, s->radius);
	printf("--color: x=%.2f y=%.2f z=%.2f\n", s->color.x, s->color.y,
		s->color.z);
	printf("--specular: %.2f\n", s->specular);
	printf("--shininess: %.2f\n", s->shininess);
}

static void	print_plane(t_object *p)
{
	printf("Object type: PLANE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f\n",
		p->position.x, p->position.y, p->position.z);
	printf("--normal: x=%.2f y=%.2f z=%.2f\n",
		p->direction.x, p->direction.y, p->direction.z);
	printf("--color: x=%.2f y=%.2f z=%.2f (checkered: ",
		p->color.x, p->color.y, p->color.z);
	if (p->checker)
		printf("true)\n");
	else
		printf("false)\n");
	printf("--specular: %.2f\n", p->specular);
	printf("--shininess: %.2f\n", p->shininess);
}

void	print_obj_list(void *o)
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
	if (obj->type == OBJ_LIGHT)
		print_light(obj);
}

void	print_sky(t_object *sky)
{
	printf("Object type: SKY\n--color: x=%.2f y=%.2f z=%.2f\n",
		sky->color.x, sky->color.y, sky->color.z);
}
