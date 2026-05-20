/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 20:44:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/20 18:15:26 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	bvh_init(t_bvh *bvh, int nb_objects)
{
	bvh->first_index = 0;
	bvh->nb_elements = nb_objects;
	bvh->objects = ft_calloc(sizeof(t_object *), nb_objects);
	bvh->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
}

t_bvh	*build_bvh_tree(t_scene *scene)
{
	t_bvh	*bvh;
	t_list	*bvh_objects;
	t_list	*tmp;
	t_vec3	left_bounds;
	int		i;

	bvh = ft_calloc(1, sizeof(t_bvh));
	if (!bvh)
		return (NULL);
	bvh_objects = ft_lstfilter(scene->objects, is_bvh_object, free_object);
	bvh_init(bvh, ft_lstsize(bvh_objects));
	i = 0;
	tmp = bvh_objects;
	while (i < bvh->nb_elements)
	{
		bvh->objects[i] = (t_object *)tmp->content;
		bvh_grow_to_include(bvh, bvh->objects[i]);
		i++;
		tmp = tmp->next;
	}
	left_bounds = get_left_bounds(bvh);
	bvh_split(bvh, left_bounds, 0);
	ft_lstclear(&bvh_objects, NULL);
	return (bvh);
}

static void	bvh_destroy_nodes(t_bvh *bvh)
{
	if (!bvh)
		return ;
	bvh_destroy_nodes(bvh->left);
	bvh_destroy_nodes(bvh->right);
	free(bvh);
}

void	bvh_destroy_tree(t_bvh **bvh)
{
	if (!*bvh)
		return ;
	free((*bvh)->objects);
	bvh_destroy_nodes(*bvh);
	(*bvh)->objects = NULL;
	*bvh = NULL;
}

void	rebuild_bvh_tree(t_bvh **bvh, t_scene *scene)
{
	bvh_destroy_tree(bvh);
	*bvh = build_bvh_tree(scene);
}
