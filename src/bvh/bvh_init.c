/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:13:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/11 19:49:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	create_bvh_tree_node(t_bvh *bvh)
{
	bvh->left = ft_calloc(1, sizeof(t_bvh));
	if (!bvh->left)
		return (0);
	bvh->right = ft_calloc(1, sizeof(t_bvh));
	if (!bvh->right)
	{
		free(bvh->left);
		return (0);
	}
	bvh->left->first_index = bvh->first_index;
	bvh->right->first_index = bvh->first_index;
	bvh->left->objects = bvh->objects;
	bvh->right->objects = bvh->objects;
	bvh->left->depth = bvh->depth + 1;
	bvh->right->depth = bvh->depth + 1;
	bvh->left->aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
	bvh->left->aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
	bvh->right->aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
	bvh->right->aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
	return (1);
}

void	bvh_init(t_bvh *bvh, t_array objects)
{
	bvh->first_index = 0;
	bvh->nb_elements = objects.len;
	bvh->objects = objects;
	bvh->aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
	bvh->aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
}
