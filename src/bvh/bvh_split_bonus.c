/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/18 18:30:13 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 22:33:16 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include <pthread.h>

void	sah_partition(t_bvh *node, t_sah *sah);

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

void	*bvh_split_thread(void *data)
{
	bvh_split((t_bvh *)data);
	return (NULL);
}

int	bvh_split_down(t_bvh *bvh)
{
	pthread_t	th[2];
	int			status;

	status = 1;
	if (bvh->depth < 3 && bvh->nb_elements >= 16)
	{
		pthread_create(&th[0], NULL, &bvh_split_thread, (void *)bvh->left);
		pthread_create(&th[1], NULL, &bvh_split_thread, (void *)bvh->right);
		pthread_join(th[0], NULL);
		pthread_join(th[1], NULL);
	}
	else
	{
		status &= bvh_split(bvh->left);
		status &= bvh_split(bvh->right);
	}
	return (status);
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
	status = bvh_split_down(bvh);
	if (status)
		bvh_remove_empty_children(bvh);
	return (status);
}
