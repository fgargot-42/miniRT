/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:39:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 22:51:17 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <unistd.h>

void	init_scene(char *file, t_scene *scene)
{
	int	parse_status;

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
}

void free_scene(t_scene *scene)
{
    ft_lstclear(&scene->spheres, free);
    ft_lstclear(&scene->planes, free);
    ft_lstclear(&scene->cylinder, free);
	ft_lstclear(&scene->lights, free);
	free(scene->cam);
	free(scene->ambient);
    free(scene);
}
