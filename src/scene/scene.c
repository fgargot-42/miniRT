/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 22:39:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/20 00:35:23 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"
#include <unistd.h>

static void	set_default_sky(t_scene *scene)
{
	t_object	*default_sky;

	default_sky = ft_calloc(1, sizeof(t_object));
	if (!default_sky)
	{
		free_scene(scene);
		exit(1);
	}
	default_sky->color = (t_vec3){0, 0, 0};
	if (!scene->sky)
		scene->sky = default_sky;
	else
		free(default_sky);
}

static void	print_bvh_tree(t_bvh *bvh, int depth)
{
	int	i;

	if (!bvh)
		return ;
	i = depth;
	while (--i > 0)
		printf("    ");
	printf("BVH\n");
	i = depth;
	while (--i > 0)
		printf("    ");
	if (depth)
		printf("│\n");
	i = depth;
	while (--i > 0)
		printf("    ");
	if (depth)
		printf("└───");
	printf("Box at: [% .3f % .3f % .3f ], [% .3f % .3f % .3f ]\n",
		bvh->aabb_min.x, bvh->aabb_min.y, bvh->aabb_min.z,
		bvh->aabb_max.x, bvh->aabb_max.y, bvh->aabb_max.z);
	i = depth;
	while (i--)
		printf("    ");
	printf("starting_index: %d, nb_elements: %d\n", bvh->first_index, bvh->nb_elements);
	if (bvh->left)
		print_bvh_tree(bvh->left, depth + 1);
	if (bvh->right)
		print_bvh_tree(bvh->right, depth + 1);
}

void	init_scene(char *file, t_scene *scene)
{
	int		parse_status;

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
	scene->bvh = build_bvh_tree(scene);
#if DEBUG
	print_bvh_tree(scene->bvh, 0);
#endif // DEBUG
	set_default_sky(scene);
}

void	free_object(void *object)
{
	t_object	*o;

	o = (t_object *)object;
	if (o->type >= OBJ_CYLINDER && o->type != OBJ_TRIANGLE
		&& o->props.transform_axis)
		free_matrix(o->props.transform_axis);
	free(o);
}

void	free_scene(t_scene *scene)
{
	ft_lstclear(&scene->objects, free_object);
	ft_lstclear(&scene->lights, free_object);
	free(scene->cam);
	free(scene->ambient);
	free(scene->sky);
	free(scene);
}
