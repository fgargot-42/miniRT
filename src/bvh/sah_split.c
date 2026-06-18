/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sah_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 17:04:05 by fgargot           #+#    #+#             */
/*   Updated: 2026/06/18 17:16:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

#define BINS 64
typedef struct s_bin
{
	t_aabb	aabb;
	int		count;
}	t_bin;

static void	aabb_grow_to_include(t_aabb *aabb, t_object *obj)
{
	t_aabb	obj_box;

	get_object_aabb(obj, &obj_box);
	aabb->min = vec3_min(aabb->min, obj_box.min);
	aabb->max = vec3_max(aabb->max, obj_box.max);
}

static void	aabb_grow_to_include_center(t_aabb *aabb, t_object *obj)
{
	t_vec3	center;

	center = get_object_center(obj);
	aabb->min = vec3_min(aabb->min, center);
	aabb->max = vec3_max(aabb->max, center);
}

static double	get_aabb_area(t_aabb aabb)
{
	t_vec3	e;

	e = vec3_sub(aabb.max, aabb.min);
	return ((e.x * e.y + e.x * e.z + e.y * e.z));
}

static double	get_sah_cost(t_aabb box, int count)
{
	return (get_aabb_area(box) * count);
}

void	sah_partition(t_bvh *node, t_sah *sah)
{
	int			i;
	int			j;

	if (sah->axis == -1 || !sah->count_l || !sah->count_r)
		return ;
	i = node->first_index;
	j = node->first_index + node->nb_elements - 1;
	while (i <= j)
	{
		while (i <= j && get_object_center(node->objects.array[i]).vec[sah->axis] < sah->pos)
			i++;
		while (i <= j && get_object_center(node->objects.array[j]).vec[sah->axis] >= sah->pos)
			j--;
		if (i < j)
			ft_memswap(&node->objects.array[i], &node->objects.array[j],
					sizeof(t_object *));
	}
	sah->count_l = i - node->first_index;
	sah->count_r = node->nb_elements - (i - node->first_index);
}

// 0 is left, 1 is right
static void	get_plane_area_count(double area[2][BINS - 1], int count[2][BINS - 1], t_bin *bins)
{
	t_aabb	box[2];
	int		sum[2];
	int		i;

	i = 0;
	sum[0] = 0;
	sum[1] = 0;
	box[0].min = (t_vec3){{1e30, 1e30, 1e30}};
	box[0].max = (t_vec3){{-1e30, -1e30, -1e30}};
	box[1].min = (t_vec3){{1e30, 1e30, 1e30}};
	box[1].max = (t_vec3){{-1e30, -1e30, -1e30}};
	while (i < BINS - 1)
	{
		sum[0] += bins[i].count;
		count[0][i] = sum[0];
		box[0].min = vec3_min(box[0].min, bins[i].aabb.min);
		box[0].max = vec3_max(box[0].max, bins[i].aabb.max);
		area[0][i] = get_aabb_area(box[0]);
		sum[1] += bins[BINS - 1 - i].count;
		count[1][BINS - 2 - i] = sum[1];
		box[1].min = vec3_min(box[1].min, bins[BINS - 1 - i].aabb.min);
		box[1].max = vec3_max(box[1].max, bins[BINS - 1 - i].aabb.max);
		area[1][BINS - 2 - i] = get_aabb_area(box[1]);
		i++;
	}
}

static double	evaluate_sah_cost(t_bin *bins, int *split_idx, int *count_l)
{
	double	area[2][BINS - 1];
	int		count[2][BINS - 1];
	double	best_cost;
	double	plane_cost;
	int		i;

	best_cost = 1e30;
	get_plane_area_count(area, count, bins);
	i = 0;
	while (i < BINS - 1)
	{
		plane_cost = count[0][i] * area[0][i] + count[1][i] * area[1][i];
		if (plane_cost < best_cost)
		{
			best_cost = plane_cost;
			*split_idx = i;
			*count_l = count[0][i];
		}
		i++;
	}
	return (best_cost);
}

static t_bin *populate_bins(t_bvh *node, int axis, t_aabb bounds)
{
	t_bin		*bins;
	int			index;
	int			bin_idx;
	double		scale;
	t_object	*obj;

	index = 0;
	if (bounds.min.vec[axis] == bounds.max.vec[axis])
		return (NULL);
	scale = BINS / (bounds.max.vec[axis] - bounds.min.vec[axis]);
	if (scale < 0)
		return (NULL);
	bins = ft_calloc(BINS, sizeof(t_bin));
	if (!bins)
		return (NULL);
	while (index < BINS)
	{
		bins[index].aabb.min = (t_vec3){{1e30, 1e30, 1e30}};
		bins[index].aabb.max = (t_vec3){{-1e30, -1e30, -1e30}};
		index++;
	}
	index = 0;
	while (index < node->nb_elements)
	{
		obj = node->objects.array[index + node->first_index];
		bin_idx = fmin(BINS - 1, (int)((get_object_center(obj).vec[axis]
						- bounds.min.vec[axis]) * scale));
		bins[bin_idx].count++;
		aabb_grow_to_include(&bins[bin_idx].aabb, obj);
		index++;
	}
	return (bins);
}

static void	find_best_split_plane(t_bvh *node, t_aabb bounds, int axis, t_sah *sah)
{
	t_bin	*bins;
	double	cost;
	double	scale;
	int		count_l;
	int		split_idx;
	
	split_idx = -1;
	count_l = -1;
	bins = populate_bins(node, axis, bounds);
	if (!bins)
		return ;
	scale = (bounds.max.vec[axis] - bounds.min.vec[axis]) / BINS;
	cost = evaluate_sah_cost(bins, &split_idx, &count_l);
	if (sah->cost > cost)
	{
		sah->cost = cost;
		sah->axis = axis;
		sah->pos = bounds.min.vec[axis] + scale * (split_idx + 1);
		sah->count_l = count_l;
		sah->count_r = node->nb_elements - count_l;
	}
}

t_sah	get_sah_split(t_bvh *node)
{
	int		i;
	t_sah	sah;
	t_aabb	bounds;

	i = 0;
	ft_bzero(&sah, sizeof(t_sah));
	sah.cost = 1e30;
	bounds.min = (t_vec3){{1e30, 1e30, 1e30}};
	bounds.max = (t_vec3){{-1e30, -1e30, -1e30}};
	while (i < node->nb_elements)
	{
		aabb_grow_to_include_center(&bounds,
				node->objects.array[i + node->first_index]);
		i++;
	}
	i = 0;
	while (i < 3)
	{
		find_best_split_plane(node, bounds, i, &sah);
		i++;
	}
	if (sah.cost >= get_sah_cost(node->aabb, node->nb_elements))
		sah.axis = -1;
	return (sah);
}
