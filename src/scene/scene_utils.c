/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/16 22:01:57 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 19:11:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdlib.h>

void	check_scene_mandatory_object(void *obj, char *obj_str, t_scene *scene)
{
	if (!obj)
	{
		ft_putstr_fd("Error\nmissing object in scene: ", 2);
		ft_putendl_fd(obj_str, 2);
		free_scene(scene);
		exit(1);
	}
}
