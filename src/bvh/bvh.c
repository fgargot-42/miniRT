/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 20:44:12 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/10 21:26:09 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "bvh.h"

static void	bvh_destroy_nodes(t_bvh *bvh)
{
	if (!bvh)
		return ;
	bvh_destroy_nodes(bvh->left);
	bvh_destroy_nodes(bvh->right);
	free(bvh);
}

void	bvh_destroy_tree(t_bvh **bvh)
{
	if (!*bvh)
		return ;
	bvh_destroy_nodes(*bvh);
	*bvh = NULL;
}

void	rebuild_bvh_tree(t_bvh **bvh, t_scene *scene)
{
	bvh_destroy_tree(bvh);
	*bvh = build_bvh_tree(scene);
}

t_bvh	*build_bvh_tree(t_scene *scene)
{
	double	time_start;
	double	time_end;
	t_bvh	*bvh;
	int		i;

	time_start = get_time();
	bvh = ft_calloc(1, sizeof(t_bvh));
	if (!bvh)
		return (NULL);
	bvh_init(bvh, scene->bvh_objects);
	i = 0;
	while (i < bvh->nb_elements)
	{
		bvh_grow_to_include(bvh, bvh->objects.array[i]);
		i++;
	}
	bvh_split(bvh);
	time_end = get_time() - time_start;
	printf("BVH built in %.3f ms\n", 1000 * time_end);
	return (bvh);
}
