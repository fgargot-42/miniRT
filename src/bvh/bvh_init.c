/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 18:13:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/01 18:22:27 by fgargot          ###   ########.fr       */
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
	bvh->left->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->left->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	bvh->right->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->right->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	return (1);
}

void	bvh_init(t_bvh *bvh, int nb_objects)
{
	bvh->first_index = 0;
	bvh->nb_elements = nb_objects;
	bvh->objects = ft_calloc(sizeof(t_object *), nb_objects);
	bvh->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
}
