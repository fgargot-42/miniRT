/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 21:14:34 by fgargot          ###   ########.fr       */
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

static t_vec3	get_bvh_center_bound(t_bvh *bvh, t_vec3 bound, t_vec3 parent_bound)
{
	t_vec3	center_obj[2];
	t_vec3	center;
	t_vec3	check[2];

	bound = vec3_add(bound, bvh->aabb_min);
	center_obj[0] = get_object_center(bvh->objects[bvh->first_index]);
	center_obj[1] = get_object_center(bvh->objects[bvh->first_index
			+ bvh->nb_elements - 1]);
	center = get_object_center(bvh->objects[bvh->first_index
			+ bvh->nb_elements / 2 - 1]);
	if (!(bvh->nb_elements & 2))
		center = vec3_scale(vec3_add(center, get_object_center(bvh->objects[bvh->first_index
			+ bvh->nb_elements / 2])), 0.5);
	check[0] = vec3_sub(center_obj[0], bound);
	check[1] = vec3_sub(center_obj[1], bound);
	if ((check[0].x > 0 && check[0].y > 0 && check[0].z > 0)
			|| (check[1].x < 0 && check[1].y < 0 && check[1].z < 0))
	{
		check[0] = vec3_sub(bvh->aabb_max, bvh->aabb_min);
		if (bound.x == parent_bound.x && check[0].x > check[0].y && check[0].x > check[0].z)
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
	//left_bound = get_bvh_center_bound(bvh, left_bound);
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

/*static t_vec3	get_split_bound_obj(t_bvh *bvh)
{
	t_vec3	obj_center;
	t_vec3	bvh_center;

	obj_center = get_object_center(
		bvh->objects[bvh->first_index + bvh->nb_elements / 2]);
	bvh_center = vec3_scale(vec3_add(bvh->aabb_min, bvh->aabb_max), 1.0 / 2.0);
	if (bvh_center.x > bvh_center.y && bvh_center.x > bvh_center.z)
		bvh_center.x = obj_center.x;
	else if (bvh_center.y >  bvh_center.z)
		bvh_center.y = obj_center.y;
	else
		bvh_center.z = obj_center.z;
	return (bvh_center);
}*/

static int	bvh_split_children(t_bvh *bvh, t_vec3 parent_bound, int depth)
{
	int		status;
	t_vec3	split_bound;

	status = 1;
	if (bvh->left)
	{
		split_bound = get_left_bounds(bvh->left);
		if (bvh->left->nb_elements == bvh->nb_elements)
			split_bound = get_bvh_center_bound(bvh->left, split_bound, parent_bound);
		status &= bvh_split(bvh->left, split_bound, depth + 1);
	}
	if (bvh->right)
	{
		split_bound = get_left_bounds(bvh->right);
		if (bvh->right->nb_elements == bvh->nb_elements)
			split_bound = get_bvh_center_bound(bvh->right, split_bound, parent_bound);
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
