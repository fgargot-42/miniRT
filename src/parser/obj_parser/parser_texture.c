/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 21:24:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 22:56:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "miniRT.h"
#include "parser.h"

int	parse_mat_color_tex(char *line, t_material *mat, t_parser_ctx *ctx)
{
	char	**split;
	char	*path;

	if (!line || !mat)
		return (0);
	if (mat->color_tex)
		return (1);
	split = ft_split_by_whitespace(line);
	if (!split)
		return (0);
	if (check_array_size(split, 2, "map_Kd", ctx->line_nb))
	{
		free_str_array(split);
		return (0);
	}
	path = ft_strjoin(ctx->rt_path, split[1]);
	mat->color_tex = load_texture(path, ctx->mlx);
	free(path);
	return (mat->color_tex != NULL);
}
