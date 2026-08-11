/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 01:33:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/11 23:48:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"
#include <unistd.h>

static int	ft_no_bvh_obj(void *e)
{
	t_object	*obj;

	obj = (t_object *)e;
	return (obj->type <= OBJ_PLANE);
}

static bool	init_scene_arrays(t_scene *scene)
{
	if (!scene)
		return (false);
	scene->objects = ft_arraynew();
	scene->lights = ft_arraynew();
	scene->mat = ft_arraynew();
	if (!scene->objects.array || !scene->lights.array || !scene->mat.array)
		return (false);
	return (true);
}

void	init_scene(char *file, t_data *data)
{
	bool	parse_status;
	t_array	new_obj;

	ft_bzero(data->scene, sizeof(t_scene));
	parse_status = init_scene_arrays(data->scene);
	parse_status &= parse_scene(file, data);
	if (!parse_status)
	{
		free_scene(data->scene, data->mlx);
		exit(1);
	}
	check_scene_mandatory_object(data->scene->ambient, "ambient lighting",
		data->scene, data->mlx);
	check_scene_mandatory_object(data->scene->cam, "camera",
		data->scene, data->mlx);
	check_scene_array_not_empty(&data->scene->lights, "light",
		data->scene, data->mlx);
	data->scene->bvh_objects = ft_array_filter(data->scene->objects,
			is_bvh_object, free_object);
	data->scene->bvh = build_bvh_tree(data->scene);
	data->scene->bvh_display_level = -1;
	new_obj = ft_array_filter(data->scene->objects, ft_no_bvh_obj, free_object);
	free(data->scene->objects.array);
	data->scene->objects = new_obj;
	set_default_sky(data->scene, data->mlx);
}

void	free_object(void *object)
{
	t_object	*o;

	o = (t_object *)object;
	free(o);
}

void	free_scene(t_scene *scene, mlx_context mlx)
{
	ft_arrayclear(&scene->objects, free_object);
	ft_arrayclear(&scene->bvh_objects, free_object);
	ft_arrayclear(&scene->lights, free_object);
	ft_arrayclear(&scene->mat, destroy_material);
	if (scene->skybox)
	{
		if (scene->skybox->data)
			mlx_destroy_image(mlx, scene->skybox->data);
		free(scene->skybox);
	}
	if (scene->bvh)
		bvh_destroy_tree(&scene->bvh);
	free(scene->cam);
	free(scene->ambient);
	free(scene->sky);
	ft_bzero(scene, sizeof(t_scene));
	free(scene);
}
