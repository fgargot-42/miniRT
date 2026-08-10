/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 21:24:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/10 19:02:37 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "miniRT_bonus.h"
#include "parser_bonus.h"

static char	*get_tex_path(char *rt_path, char *tex_path)
{
	char	*res_path;

	if (!rt_path || !tex_path)
		return (NULL);
	if (tex_path[0] == '/')
		res_path = ft_strdup(tex_path);
	else
		res_path = ft_strjoin(rt_path, tex_path);
	return (res_path);
}

static bool	parse_tex_vector(char **split, t_vec3 *vec, char *param,
	int line_nb)
{
	bool	parse_result;

	parse_result = parse_double(split[1], &vec->x, param, line_nb);
	parse_result &= parse_double(split[2], &vec->y, param, line_nb);
	parse_result &= parse_double(split[3], &vec->z, param, line_nb);
	return (parse_result);
}

static int	parse_tex_option(char **split, t_texture *tex, int line_nb)
{
	bool	parse_result;
	int		i;

	parse_result = true;
	i = 1;
	while (split[i][0] == '-')
	{
		if (!ft_strcmp(split[i], "-s"))
		{
			parse_result = parse_tex_vector(&split[i], &(tex->scale), "-s",
					line_nb);
			i += 4;
		}
		else if (!ft_strcmp(split[i], "-o") || !ft_strcmp(split[i], "-t"))
			i += 4;
		else
			i += 2;
	}
	if (!parse_result)
		i = -1;
	return (i);
}

int	parse_mat_tex(char **split, t_texture **tex, t_parser_ctx *ctx, char *param)
{
	char	*path;
	char	*filename;
	int		i;

	if (!split || !split[0] || !split[1] || !param || !tex)
		return (tex != NULL);
	if (!*tex)
		*tex = new_texture();
	if (!*tex)
		return (0);
	i = parse_tex_option(split, *tex, ctx->line_nb);
	if (i == -1 || !split[i][0])
	{
		if (i != -1)
			print_parse_error("missing parameter(s)", param, ctx->line_nb);
		return (0);
	}
	filename = ft_strjoin_array((const char **)&split[i], " ");
	if (!filename)
		return (0);
	path = get_tex_path(ctx->rt_path, filename);
	free(filename);
	load_texture(path, *tex, ctx->mlx);
	free(path);
	return ((*tex)->data != NULL);
}

int	parse_obj_tex_file(t_object_model *obj, char *rt_path,
		char *tex_file, void *mlx)
{
	char		*tex_path;
	t_texture	*tex;
	bool		is_tex_loaded;

	if (!tex_file || tex_file[0] == '\0')
		return (1);
	tex_path = get_tex_path(rt_path, tex_file);
	if (!tex_path)
		return (0);
	tex = new_texture();
	if (!tex)
		return (0);
	is_tex_loaded = load_texture(tex_path, tex, mlx);
	if (!is_tex_loaded)
	{
		free(tex);
		return (0);
	}
	obj->tex = tex;
	return (1);
}
