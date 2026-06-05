/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 20:44:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/05 15:00:54 by fgargot          ###   ########.fr       */
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
	if (!*bvh)
		return ;
	free((*bvh)->objects);
	(*bvh)->objects = NULL;
	bvh_destroy_nodes(*bvh);
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
	t_list	*tmp;
	int		i;

	bvh = ft_calloc(1, sizeof(t_bvh));
	if (!bvh)
		return (NULL);
	bvh_init(bvh, ft_lstsize(scene->bvh_objects));
	i = 0;
	tmp = scene->bvh_objects;
	while (i < bvh->nb_elements)
	{
		bvh->objects[i] = (t_object *)tmp->content;
		bvh_grow_to_include(bvh, bvh->objects[i]);
		i++;
		tmp = tmp->next;
	}
	bvh_split(bvh);
	return (bvh);
}
