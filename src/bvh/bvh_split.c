/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/03 17:07:55 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	split_bvh_node(t_bvh *bvh)
{
	int		i;
	t_vec3	center;
	t_bvh	*child;
	t_vec3	range;

	i = bvh->first_index;
	range = vec3_add(get_left_bounds(bvh), bvh->aabb_min);
	while (i < bvh->nb_elements + bvh->first_index)
	{
		center = get_object_center(bvh->objects[i]);
		child = bvh->left;
		if ((center.x > range.x) || (center.y > range.y)
			|| (center.z > range.z))
			child = bvh->right;
		if (child == bvh->left)
			bvh->right->first_index++;
		child->nb_elements++;
		bvh_grow_to_include(child, bvh->objects[i]);
		i++;
	}
	sort_bvh_objects(bvh, range);
	return (1);
}

int	bvh_split(t_bvh *bvh)
{
	int		status;
	int		count;
	t_vec3	range;

	status = 1;
	if (!bvh || bvh->depth == BVH_DEPTH || bvh->nb_elements <= 1)
		return (1);
	range = get_left_bounds(bvh);
	count = count_elements_split_right(bvh, range);
	if (count == 0 || count == bvh->nb_elements)
		return (status);
	status &= create_bvh_tree_node(bvh);
	status &= split_bvh_node(bvh);
	status &= bvh_split(bvh->left);
	status &= bvh_split(bvh->right);
	if (status)
		bvh_remove_empty_children(bvh);
	return (status);
}
