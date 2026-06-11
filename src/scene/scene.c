/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD
/*   Created: 2026/06/15 19:29:40 by mabarrer          #+#    #+#             */
/*   Updated: 2026/06/15 19:29:58 by mabarrer         ###   ########.fr       */
=======
/*   Created: 2026/04/13 22:39:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/11 20:02:41 by fgargot          ###   ########.fr       */
>>>>>>> 246dc3e (Added: BVH building with SAH + changed t_vec structs to union to access them as arrays)
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include "parser.h"
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

void	print_bvh_tree(t_bvh *bvh, int depth)
{
	int	i;

	if (!bvh)
		return ;
	i = depth;
	while (i-- > 0)
		printf("    ");
	printf("BVH\n");
	i = depth;
	while (i-- > 0)
		printf("    ");
	printf("│\n");
	i = depth;
	while (i-- > 0)
		printf("    ");
	printf("└───");
	printf("Box at: [% .3f % .3f % .3f ], [% .3f % .3f % .3f ]\n",
		bvh->aabb.min.x, bvh->aabb.min.y, bvh->aabb.min.z,
		bvh->aabb.max.x, bvh->aabb.max.y, bvh->aabb.max.z);
	i = depth + 1;
	while (i--)
		printf("    ");
	printf("starting_index: %d, nb_elements: %d\n",
		bvh->first_index, bvh->nb_elements);
	if (bvh->left)
		print_bvh_tree(bvh->left, depth + 1);
	if (bvh->right)
		print_bvh_tree(bvh->right, depth + 1);
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
	if (!data->scene->ambient)
	{
		ft_putstr_fd("Error\nmissing object in scene: ambient lighting\n", 2);
		free_scene(data->scene, data->mlx);
		exit(1);
	}
	if (!data->scene->cam)
	{
		ft_putstr_fd("Error\nmissing object in scene: camera\n", 2);
		free_scene(data->scene, data->mlx);
		exit(1);
	}
	data->scene->bvh_objects = ft_array_filter(data->scene->objects,
			is_bvh_object, free_object);
	data->scene->bvh = build_bvh_tree(data->scene);
	new_obj = ft_array_filter(data->scene->objects, ft_no_bvh_obj, free_object);
	free(data->scene->objects.array);
	data->scene->objects = new_obj;
	set_default_sky(data->scene, data->mlx);
}

void	free_object(void *object)
{
	t_object	*o;

	o = (t_object *)object;
	if (o->type >= OBJ_CYLINDER && o->type != OBJ_TRIANGLE
		&& o->props.transform_axis)
		free_matrix(o->props.transform_axis);
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
