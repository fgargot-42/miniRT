/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:39:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 19:13:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <unistd.h>

static void	set_default_sky(t_scene *scene)
{
	t_vec3	*default_sky;

	default_sky = malloc(sizeof(t_vec3));
	if (!default_sky)
	{
		free_scene(scene);
		exit(1);
	}
	default_sky->x = 0;
	default_sky->y = 0;
	default_sky->z = 0;
	if (!scene->sky)
		scene->sky = default_sky;
	else
		free(default_sky);
}

void	init_scene(char *file, t_scene *scene)
{
	int		parse_status;

	ft_bzero(scene, sizeof(t_scene));
	parse_status = parse_scene(file, scene);
	if (!parse_status)
	{
		free_scene(scene);
		exit(1);
	}
	if (!scene->ambient)
	{
		ft_putstr_fd("Error\nmissing object in scene: ambient lighting\n", 2);
		free_scene(scene);
		exit(1);
	}
	if (!scene->cam)
	{
		ft_putstr_fd("Error\nmissing object in scene: camera\n", 2);
		free_scene(scene);
		exit(1);
	}
	set_default_sky(scene);
}

static void free_object(void *object)
{
	t_object	*o;

	o = (t_object *)object;
	if (o->type == OBJ_CYLINDER)
		free_matrix(((t_cylinder *)o->object)->transform_axis);
	if (o->type == OBJ_CONE)
		free_matrix(((t_cone *)o->object)->transform_axis);
	free(o->object);
	free(o);
}

void	free_scene(t_scene *scene)
{
	ft_lstclear(&scene->objects, free_object);
	free(scene->cam);
	free(scene->ambient);
	free(scene->sky);
	free(scene);
}
