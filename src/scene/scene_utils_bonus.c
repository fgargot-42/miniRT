/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:01:57 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 19:13:22 by fgargot          ###   ########.fr       */
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
