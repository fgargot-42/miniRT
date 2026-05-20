/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_size.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 17:54:19 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/21 00:09:18 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "veclib.h"

void	bvh_grow_to_include(t_bvh *bvh, t_object *object)
{
	t_vec3						aabb_min;
	t_vec3						aabb_max;
	static const t_obj_aabb_fn	get_aabb[] = {NULL, NULL, NULL, NULL, NULL,
		get_sphere_aabb, get_cylinder_aabb, get_cone_aabb,
		get_hyperboloid_aabb, get_paraboloid_aabb, get_triangle_aabb};

	aabb_min = (t_vec3){1e30, 1e30, 1e30};
	aabb_max = (t_vec3){-1e30, -1e30, -1e30};
	if (object->type >= OBJ_SPHERE)
		get_aabb[object->type](object, &aabb_min, &aabb_max);
	bvh->aabb_min = vec3_min(bvh->aabb_min, aabb_min);
	bvh->aabb_max = vec3_max(bvh->aabb_max, aabb_max);
}

void	bvh_grow_all_to_include(t_bvh *root, t_object *object)
{
	int		i;
	t_bvh	*node;

	i = 0;
	while (i < root->nb_elements)
	{
		if (root->objects[i] == object)
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
