/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 21:05:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/08 23:08:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

void	add_triangles_to_scene(t_scene *scene, t_array triangles)
{
	size_t	i;

	i = 0;
	while (i < triangles.len)
	{
		ft_arrayadd_back(&scene->objects, triangles.array[i], free_object);
		i++;
	}
}
