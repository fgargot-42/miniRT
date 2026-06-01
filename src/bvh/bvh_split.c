/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/01 18:57:56 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	get_obj_center_from_bound(t_bvh *bvh, t_vec3 *center_min,
		t_vec3 *center_max, t_vec3 *bound)
{
	*bound = vec3_add(*bound, bvh->aabb_min);
	*center_min = get_object_center(bvh->objects[bvh->first_index]);
	*center_max = get_object_center(bvh->objects[bvh->first_index
			+ bvh->nb_elements - 1]);
	*center_min = vec3_sub(*center_min, *bound);
	*center_max = vec3_sub(*center_max, *bound);
}

static t_vec3	get_bvh_center_bound(t_bvh *bvh, t_vec3 bound,
		t_vec3 parent_bound)
{
	t_vec3	check[2];
	t_vec3	center;

	ft_bzero(check, 2 * sizeof(t_vec3));
	get_obj_center_from_bound(bvh, &check[0], &check[1], &bound);
	center = get_object_center(bvh->objects[bvh->first_index
			+ bvh->nb_elements / 2 - 1]);
	if (!(bvh->nb_elements & 2))
		center = vec3_scale(vec3_add(center,
					get_object_center(bvh->objects[bvh->first_index
						+ bvh->nb_elements / 2])), 0.5);
	if ((check[0].x > 0 && check[0].y > 0 && check[0].z > 0)
		|| (check[1].x < 0 && check[1].y < 0 && check[1].z < 0))
	{
		check[0] = vec3_sub(bvh->aabb_max, bvh->aabb_min);
		if (bound.x == parent_bound.x && check[0].x > check[0].y
			&& check[0].x > check[0].z)
			bound.x = center.x / 2.0;
		else if (bound.y == parent_bound.y && check[0].y > check[0].z)
			bound.y = center.y / 2.0;
		else
			bound.z = center.z / 2.0;
	}
	return (bound);
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
	while (i < bvh->nb_elements + bvh->first_index)
	{
		center = get_object_center(bvh->objects[i]);
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

static int	bvh_split_children(t_bvh *bvh, t_vec3 parent_bound, int depth)
{
	int		status;
	t_vec3	split_bound;

	status = 1;
	if (bvh->left)
	{
		split_bound = get_left_bounds(bvh->left);
		if (bvh->left->nb_elements == bvh->nb_elements)
			split_bound = get_bvh_center_bound(bvh->left,
					split_bound, parent_bound);
		status &= bvh_split(bvh->left, split_bound, depth + 1);
	}
	if (bvh->right)
	{
		split_bound = get_left_bounds(bvh->right);
		if (bvh->right->nb_elements == bvh->nb_elements)
			split_bound = get_bvh_center_bound(bvh->right,
					split_bound, parent_bound);
		status &= bvh_split(bvh->right, split_bound, depth + 1);
	}
	return (status);
}

int	bvh_split(t_bvh *bvh, t_vec3 split_bound, int depth)
{
	int		status;

	status = 1;
	if (!bvh || depth == BVH_DEPTH || bvh->nb_elements <= 1)
		return (1);
	split_bvh_node(bvh, split_bound);
	status = bvh_split_children(bvh, split_bound, depth);
	return (status);
}
