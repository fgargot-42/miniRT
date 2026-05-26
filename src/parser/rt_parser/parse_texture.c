/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 22:32:06 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/27 00:29:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "parser.h"

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
	if (!tex)
		return (0);
	obj->tex = tex;
	return (1);
}
