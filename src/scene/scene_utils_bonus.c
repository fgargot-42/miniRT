/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:01:57 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/11 23:50:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <stdlib.h>

void	check_scene_mandatory_object(void *obj, char *obj_str, t_scene *scene,
		mlx_context mlx)
{
	if (!obj)
	{
		ft_putstr_fd("Error\nmissing object in scene: ", 2);
		ft_putendl_fd(obj_str, 2);
		free_scene(scene, mlx);
		exit(1);
	}
}

void	check_scene_array_not_empty(t_array *arr, char *arr_str, t_scene *scene,
	mlx_context mlx)
{
	if (!arr || arr->len == 0)
	{
		ft_putstr_fd("Error\nmissing object in scene: ", 2);
		ft_putendl_fd(arr_str, 2);
		free_scene(scene, mlx);
		exit(1);
	}
}

void	set_default_sky(t_scene *scene, mlx_context mlx)
{
	t_object	*default_sky;

	default_sky = ft_calloc(1, sizeof(t_object));
	if (!default_sky)
	{
		free_scene(scene, mlx);
		exit(1);
	}
	default_sky->color = (t_vec3){{0, 0, 0}};
	if (!scene->sky)
		scene->sky = default_sky;
	else
		free(default_sky);
}
