/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 20:44:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/04 20:25:19 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

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
	int	i;

	i = 0;
	if (!*bvh)
		return ;
	while (i < (*bvh)->nb_elements)
	{
		free_object((*bvh)->objects[i]);
		i++;
	}
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

t_bvh	*build_bvh_tree(t_scene *scene)
{
	t_bvh	*bvh;
	t_list	*bvh_objects;
	t_list	*tmp;
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
	bvh_split(bvh);
	ft_lstclear(&bvh_objects, NULL);
	return (bvh);
}
