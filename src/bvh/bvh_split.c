/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 01:37:50 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	create_bvh_tree_node(t_bvh *bvh)
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

static void	bvh_remove_empty_children(t_bvh *bvh)
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

static void	sort_bvh_objects(t_bvh *bvh, t_vec3 left_bound)
{
	if (bvh->left && bvh->right && left_bound.x < bvh->aabb_max.x)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'x');
	if (bvh->left && bvh->right && left_bound.y < bvh->aabb_max.y)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'y');
	if (bvh->left && bvh->right && left_bound.z < bvh->aabb_max.z)
		sort_bvh_objects_asc(bvh->objects, bvh->first_index,
			bvh->nb_elements, 'z');
}

static int	split_bvh_node(t_bvh *bvh, t_vec3 left_bound)
{
	int		i;
	int		status;
	t_vec3	center;
	t_bvh	*child;

	status = create_bvh_tree_node(bvh);
	if (!status)
		return (0);
	i = bvh->first_index;
	//left_bound = vec3_add(left_bound, bvh->aabb_min);
	while (i < bvh->nb_elements + bvh->first_index)
	{
		center = vec3_add(get_object_center(bvh->objects[i]), bvh->aabb_min);
		//center = get_object_center(bvh->objects[i]);
		child = bvh->left;
		if ((center.x > left_bound.x) || (center.y > left_bound.y)
			|| (center.z > left_bound.z))
			child = bvh->right;
		if (child == bvh->left)
			bvh->right->first_index++;
		child->nb_elements++;
		bvh_grow_to_include(child, bvh->objects[i]);
		i++;
	}
	bvh_remove_empty_children(bvh);
	sort_bvh_objects(bvh, left_bound);
	return (1);
}

int	bvh_split(t_bvh *bvh, t_vec3 left_bound, int depth)
{
	int		status;

	status = 1;
	if (!bvh || depth == BVH_DEPTH || bvh->nb_elements <= 1)
		return (1);
	split_bvh_node(bvh, left_bound);
	if (!bvh->left || !bvh->right)
	{
		if (left_bound.x > left_bound.y && left_bound.x > left_bound.z)
			left_bound.x /= 2.0;
		else if (left_bound.y > left_bound.z)
			left_bound.y /= 2.0;
		else
			left_bound.z /= 2.0;
		status &= bvh_split(bvh->left, left_bound, depth + 1);
		status &= bvh_split(bvh->right, left_bound, depth + 1);
	}
	else
	{
		left_bound = get_left_bounds(bvh->left);
		status &= bvh_split(bvh->left, left_bound, depth + 1);
		left_bound = get_left_bounds(bvh->right);
		status &= bvh_split(bvh->right, left_bound, depth + 1);
	}
	return (status);
}
