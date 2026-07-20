/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 22:00:38 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/21 00:54:11 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H

# include "libft.h"

# define BVH_DEPTH 24
# define SAH_BINS 64

typedef struct s_scene		t_scene;
typedef struct s_ray		t_ray;
typedef struct s_hit_record	t_hit_record;
typedef struct s_object		t_object;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}	t_aabb;

typedef struct s_bvh
{
	t_aabb			aabb;
	struct s_bvh	*left;
	struct s_bvh	*right;
	t_array			objects;
	int				first_index;
	int				nb_elements;
	int				depth;
}	t_bvh;

typedef struct s_sah
{
	double	cost;
	int		axis;
	double	pos;
	int		count_l;
	int		count_r;
}	t_sah;

typedef struct s_bvh_hit_ctx
{
	t_scene			*scene;
	t_ray			*ray;
	double			*closest;
	t_hit_record	*rec;
	int				hit;
}	t_bvh_hit_ctx;

typedef struct s_bvh_state
{
	t_bvh	*stack[2 * BVH_DEPTH];
	t_bvh	*node;
	t_bvh	*child[2];
	double	dist[2];
	int		depth;
}	t_bvh_state;

typedef struct s_bin
{
	t_aabb	aabb;
	int		count;
}	t_bin;

// BVH

typedef void				(*t_obj_aabb_fn)(t_object *, t_aabb *);

void	get_sphere_aabb(t_object *obj, t_aabb *aabb);
void	get_cylinder_aabb(t_object *obj, t_aabb *aabb);

void	get_object_aabb(t_object *obj, t_aabb *aabb);

void	bvh_init(t_bvh *bvh, t_array objects);
t_bvh	*build_bvh_tree(t_scene *scene);
int		create_bvh_tree_node(t_bvh *bvh);
void	bvh_destroy_tree(t_bvh **bvh);
void	rebuild_bvh_tree(t_bvh **bvh, t_scene *scene);
void	bvh_remove_empty_children(t_bvh *bvh);

t_vec3	get_range_from_object_centers(t_bvh *bvh);
void	count_elements_split_right(t_bvh *bvh, t_sah *sah);
void	bvh_grow_to_include(t_bvh *bvh, t_object *object);
void	bvh_grow_all_to_include(t_bvh *root, t_object *object);
int		is_bvh_object(void *e);
int		bvh_split(t_bvh *bvh);
void	get_box_aabb(t_list *elements, t_vec3 *aabb_min, t_vec3 *aabb_max);
t_vec3	get_left_bounds(t_bvh *bvh);
t_vec3	get_object_center(t_object *obj);
t_sah	get_sah_split(t_bvh *node);
void	aabb_grow_to_include(t_aabb *aabb, t_object *obj);
void	aabb_grow_to_include_center(t_aabb *aabb, t_object *obj);
double	get_aabb_area(t_aabb aabb);
void	sah_partition(t_bvh *node, t_sah *sah);
t_bin	*init_bins(int axis, t_aabb bounds);

#endif // BVH_H
