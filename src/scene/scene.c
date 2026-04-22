/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:39:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/22 18:48:08 by fgargot          ###   ########.fr       */
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

static void	free_cylinder(void *cyl)
{
	t_cylinder	*elem;

	elem = (t_cylinder *)cyl;
	free_matrix(elem->transform_axis);
	free(elem);
}

static void	free_cone(void *cone)
{
	t_cone	*elem;

	elem = (t_cone *)cone;
	free_matrix(elem->transform_axis);
	free(elem);
}

void	free_scene(t_scene *scene)
{
	ft_lstclear(&scene->spheres, free);
	ft_lstclear(&scene->planes, free);
	ft_lstclear(&scene->cylinder, free_cylinder);
	ft_lstclear(&scene->cone, free_cone);
	ft_lstclear(&scene->lights, free);
	free(scene->cam);
	free(scene->ambient);
	free(scene);
}
