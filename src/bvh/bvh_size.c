/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:54:19 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/17 21:34:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

void	bvh_grow_to_include(t_bvh *bvh, t_object *object)
{
	t_aabb						aabb;

	aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
	aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
	get_object_aabb(object, &aabb);
	bvh->aabb.min = vec3_min(bvh->aabb.min, aabb.min);
	bvh->aabb.max = vec3_max(bvh->aabb.max, aabb.max);
}

void	bvh_grow_all_to_include(t_bvh *root, t_object *object)
{
	int		i;
	t_bvh	*node;

	i = 0;
	while (i < root->nb_elements)
	{
		if (root->objects.array[i] == object)
			break ;
		i++;
	}
	if (i >= root->nb_elements)
		return ;
	node = root;
	while (node)
	{
		bvh_grow_to_include(node, object);
		if (node->right && i >= node->right->first_index)
			node = node->right;
		else
			node = node->left;
	}
}

void	bvh_remove_empty_children(t_bvh *bvh)
{
	if (!bvh->left->nb_elements)
	{
		free(bvh->left);
		bvh->left = NULL;
	}
	if (!bvh->right->nb_elements)
	{
		free(bvh->right);
		bvh->right = NULL;
	}
}
