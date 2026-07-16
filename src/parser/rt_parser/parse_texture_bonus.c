/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:32:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/16 21:34:18 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"

int	parse_texture_file(char *param, t_object *obj, t_parser_ctx *ctx, void *mlx)
{
	char		*tex_path;
	t_texture	*tex;

	if (!param || param[0] == '\0')
		return (1);
	tex_path = ft_strjoin(ctx->rt_path, param);
	if (!tex_path)
		return (0);
	tex = load_texture(tex_path, mlx);
	free(tex_path);
	if (!tex)
		return (0);
	obj->tex = tex;
	return (1);
}
