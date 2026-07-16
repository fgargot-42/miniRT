/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 01:33:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/17 20:13:12 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"
#include <unistd.h>

static void	set_default_sky(t_scene *scene, mlx_context mlx)
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

static int	ft_no_bvh_obj(void *e)
{
	t_object	*obj;

	obj = (t_object *)e;
	return (obj->type <= OBJ_PLANE);
}

void	init_scene(char *file, t_data *data)
{
	int		parse_status;
	t_array	new_obj;

	ft_bzero(data->scene, sizeof(t_scene));
	data->scene->objects = ft_arraynew();
	data->scene->lights = ft_arraynew();
	data->scene->mat = ft_arraynew();
	parse_status = parse_scene(file, data);
	if (!parse_status)
	{
		free_scene(data->scene, data->mlx);
		exit(1);
	}
	check_scene_mandatory_object(data->scene->ambient, "ambient lighting",
		data->scene, data->mlx);
	check_scene_mandatory_object(data->scene->cam, "camera",
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
	if (o->tex)
	{
		mlx_destroy_image(o->tex->mlx, o->tex->data);
		free(o->tex);
	}
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
