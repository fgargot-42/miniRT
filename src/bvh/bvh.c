/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 20:44:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/18 18:11:16 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec3	get_object_center(t_object *obj)
{
	t_vec3	aabb_min;
	t_vec3	aabb_max;

	aabb_min = (t_vec3){-1e30, -1e30, -1e30};
	aabb_max = (t_vec3){1e30, 1e30, 1e30};
	if (obj->type != OBJ_PARABOLOID && obj->type != OBJ_TRIANGLE)
		return (obj->position);
	if (obj->type == OBJ_PARABOLOID)
		get_paraboloid_aabb(obj, &aabb_min, &aabb_max);
	else if (obj->type == OBJ_TRIANGLE)
		get_triangle_aabb(obj, &aabb_min, &aabb_max);
	return (vec3_scale(vec3_add(aabb_min, aabb_max), 1.0 / 2.0));
}

static t_vec3	get_bvh_center(t_bvh bvh)
{
	return (vec3_scale(vec3_add(bvh.aabb_min, bvh.aabb_max), 1.0 / 2.0));
}

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
	bvh->left->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->left->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	bvh->right->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->right->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	return (1);
}

static int	bvh_split(t_bvh *bvh, int depth)
{
	t_bvh	*child;
	t_vec3	obj_center;
	t_vec3	bvh_center;
	int		status;
	int		i;

	if (!bvh || depth == BVH_DEPTH || bvh->nb_elements <= 1)
		return (1);
	status = create_bvh_tree_node(bvh);
	i = bvh->first_index;
	if (!status)
		return (0);
	while (i < bvh->nb_elements + bvh->first_index)
	{
		obj_center = get_object_center(bvh->objects[i]);
		bvh_center = get_bvh_center(*bvh);
		child = bvh->left;
		if (obj_center.x > bvh_center.x)
			child = bvh->right;
		if (child == bvh->left)
			bvh->right->first_index++;
		child->nb_elements++;
		bvh_grow_to_include(child, bvh->objects[i]);
		i++;
	}
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
	sort_bvh_objects_asc(bvh->objects, bvh->first_index, bvh->nb_elements, 'x');
	status &= bvh_split(bvh->left, depth + 1);
	status &= bvh_split(bvh->right, depth + 1);
	return (status);
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
	bvh->first_index = 0;
	bvh->nb_elements = ft_lstsize(bvh_objects);
	bvh->objects = ft_calloc(sizeof(t_object *), bvh->nb_elements);
	bvh->aabb_min = (t_vec3){1e30, 1e30, 1e30};
	bvh->aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	i = 0;
	tmp = bvh_objects;
	while (i < bvh->nb_elements)
	{
		bvh->objects[i] = (t_object *)tmp->content;
		bvh_grow_to_include(bvh, bvh->objects[i]);
		i++;
		tmp = tmp->next;
	}
	bvh_split(bvh, 0);
	ft_lstclear(&bvh_objects, NULL);
	return (bvh);
}
