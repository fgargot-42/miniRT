/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/19 01:33:52 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/11 23:53:38 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "parser.h"
#include <unistd.h>

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
	parse_status = data->scene->objects.array != NULL;
	parse_status &= parse_scene(file, data);
	if (!parse_status)
	{
		free_scene(data->scene);
		exit(1);
	}
	check_scene_mandatory_object(data->scene->ambient, "ambient lighting",
		data->scene);
	check_scene_mandatory_object(data->scene->cam, "camera",
		data->scene);
	check_scene_mandatory_object(data->scene->light, "light",
		data->scene);
	data->scene->bvh_objects = ft_array_filter(data->scene->objects,
			is_bvh_object, free_object);
	data->scene->bvh = build_bvh_tree(data->scene);
	data->scene->bvh_display_level = -1;
	new_obj = ft_array_filter(data->scene->objects, ft_no_bvh_obj, free_object);
	free(data->scene->objects.array);
	data->scene->objects = new_obj;
}

void	free_object(void *object)
{
	t_object	*o;

	o = (t_object *)object;
	free(o);
}

void	free_scene(t_scene *scene)
{
	ft_arrayclear(&scene->objects, free_object);
	ft_arrayclear(&scene->bvh_objects, free_object);
	if (scene->bvh)
		bvh_destroy_tree(&scene->bvh);
	free(scene->cam);
	free(scene->ambient);
	free(scene->light);
	ft_bzero(scene, sizeof(t_scene));
	free(scene);
}
