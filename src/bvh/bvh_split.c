/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/20 22:08:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "bvh.h"

static void	split_bvh_node(t_bvh *bvh, t_sah *sah)
{
	int		i;

	sah_partition(bvh, sah);
	bvh->left->nb_elements = sah->count_l;
	bvh->right->first_index = bvh->first_index + sah->count_l;
	bvh->right->nb_elements = sah->count_r;
	i = bvh->first_index;
	while (i < bvh->first_index + sah->count_l)
		bvh_grow_to_include(bvh->left, bvh->objects.array[i++]);
	while (i < bvh->first_index + bvh->nb_elements)
		bvh_grow_to_include(bvh->right, bvh->objects.array[i++]);
}

int	bvh_split(t_bvh *bvh)
{
	int			status;
	t_sah		sah;

	status = 1;
	if (!bvh || bvh->depth == BVH_DEPTH || bvh->nb_elements <= 4)
		return (1);
	sah = get_sah_split(bvh);
	if (sah.count_r == 0 || sah.count_r == bvh->nb_elements)
		return (status);
	status &= create_bvh_tree_node(bvh);
	if (status)
		split_bvh_node(bvh, &sah);
	status = bvh_split(bvh->left);
	status &= bvh_split(bvh->right);
	if (status)
		bvh_remove_empty_children(bvh);
	return (status);
}
