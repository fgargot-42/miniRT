/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 21:05:51 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/21 00:29:54 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
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

void	init_object_model(t_object_model *obj)
{
	ft_bzero(obj, sizeof(obj));
	obj->materials = ft_arraynew();
	obj->triangles = ft_arraynew();
	obj->vertices = ft_arraynew();
	obj->vertex_normals = ft_arraynew();
	obj->vertex_uv = ft_arraynew();
}

void	destroy_object_model(t_object_model *obj)
{
	ft_arrayclear(&obj->triangles, NULL);
	ft_arrayclear(&obj->vertices, free);
	ft_arrayclear(&obj->vertex_normals, free);
	ft_arrayclear(&obj->vertex_uv, free);
	free(obj->materials.array);
}

void	add_materials_to_scene(t_scene *scene, t_array materials)
{
	size_t	i;

	i = 0;
	while (i < materials.len)
	{
		ft_arrayadd_back(&scene->mat, materials.array[i], destroy_material);
		i++;
	}
}
