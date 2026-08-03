/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:32:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 20:31:51 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "material.h"

t_material	*create_material(t_data *data)
{
	t_material	*new_mat;

	new_mat = new_material("");
	if (new_mat)
		ft_arrayadd_back(&data->scene->mat, new_mat, destroy_material);
	return (new_mat);
}

int	parse_texture_file(char *param, t_texture **tex, t_parser_ctx *ctx)
{
	char		*tex_path;
	bool		is_loaded;

	if (!tex || !param || param[0] == '\0' || !ft_strcmp(param, "."))
		return (tex != NULL);
	*tex = new_texture();
	if (!*tex)
		return (0);
	tex_path = ft_strjoin(ctx->rt_path, param);
	if (!tex_path)
	{
		free(*tex);
		return (0);
	}
	is_loaded = load_texture(tex_path, *tex, ctx->data->mlx);
	free(tex_path);
	return (is_loaded);
}
