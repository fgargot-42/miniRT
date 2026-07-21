/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit_bvh.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 17:38:28 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/21 21:58:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "hit.h"
#include "bvh.h"

static void	sort_bvh_children(t_bvh_state *state)
{
	if (state->dist[1] < state->dist[0])
	{
		ft_dswap(&state->dist[0], &state->dist[1]);
		ft_memswap(&state->child[0], &state->child[1], sizeof(t_bvh *));
	}
}

static void	get_bvh_children(t_bvh_hit_ctx *ctx, t_bvh_state *state)
{
	state->dist[0] = 1e30;
	state->dist[1] = 1e30;
	state->child[0] = state->node->left;
	state->child[1] = state->node->right;
	if (state->child[0])
		state->dist[0] = hit_bvh_box(state->child[0], ctx->ray, *ctx->closest);
	if (state->child[1])
		state->dist[1] = hit_bvh_box(state->child[1], ctx->ray, *ctx->closest);
	sort_bvh_children(state);
}

static int	descend_bvh(t_bvh_state *state)
{
	if (state->dist[0] == 1e30)
	{
		if (!state->depth)
			return (0);
		state->node = state->stack[--state->depth];
	}
	else
	{
		state->node = state->child[0];
		if (state->dist[1] != 1e30 && state->depth < 2 * BVH_DEPTH - 1)
			state->stack[state->depth++] = state->child[1];
	}
	return (1);
}

static int	handle_bvh_leaf(t_bvh_hit_ctx *ctx, t_bvh_state *state)
{
	if (hit_object_in_bvh(state->node, ctx->ray, ctx->closest, ctx->rec))
		ctx->hit = 1;
	if (state->depth == 0)
		return (0);
	state->node = state->stack[--state->depth];
	return (1);
}

int	hit_bvh(t_scene *scene, t_ray *ray, double *closest, t_hit_record *rec)
{
	t_bvh_hit_ctx	ctx;
	t_bvh_state		state;

	ctx = (t_bvh_hit_ctx){scene, ray, closest, rec, 0};
	state.depth = 0;
	state.node = scene->bvh;
	state.dist[0] = hit_bvh_box(state.node, ray, *closest);
	if (state.dist[0] == 1e30)
		return (0);
	while (1)
	{
		if (!state.node->left && !state.node->right)
		{
			if (!handle_bvh_leaf(&ctx, &state))
				break ;
			continue ;
		}
		if (draw_box_bounds(&ctx, state))
			ctx.hit = 1;
		get_bvh_children(&ctx, &state);
		if (!descend_bvh(&state))
			break ;
	}
	return (ctx.hit);
}
