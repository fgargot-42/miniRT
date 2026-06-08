/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/08 19:01:47 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	split_bvh_node(t_bvh *bvh)
{
	int		i;
	int		nb_left;
	t_vec3	center;
	t_vec3	range;

	i = bvh->first_index;
	nb_left = 0;
	range = vec3_add(get_left_bounds(bvh), bvh->aabb_min);
	sort_bvh_objects(bvh, range);
	while (i < bvh->nb_elements + bvh->first_index)
	{
		center = get_object_center(bvh->objects[i]);
		if ((center.x > range.x) || (center.y > range.y)
			|| (center.z > range.z))
			nb_left++;
		i++;
	}
	bvh->left->nb_elements = nb_left;
	bvh->right->first_index = bvh->first_index + nb_left;
	bvh->right->nb_elements = bvh->nb_elements - nb_left;
	i = bvh->first_index;
	while (i < bvh->first_index + nb_left)
		bvh_grow_to_include(bvh->left, bvh->objects[i++]);
	while (i < bvh->first_index + bvh->nb_elements)
		bvh_grow_to_include(bvh->right, bvh->objects[i++]);
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
	if (status)
		split_bvh_node(bvh);
	status &= bvh_split(bvh->left);
	status &= bvh_split(bvh->right);
	if (status)
		bvh_remove_empty_children(bvh);
	return (status);
}
