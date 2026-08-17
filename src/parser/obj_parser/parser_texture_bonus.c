/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/29 21:24:16 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/17 18:28:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material_bonus.h"
#include "miniRT_bonus.h"
#include "parser_bonus.h"

char	*get_tex_path(char *rt_path, char *tex_path);
bool	parse_tex_vector(char **split, t_vec3 *vec, char *param,
			int line_nb);

static bool	is_one_param_option(char *option)
{
	static const char	*options[] = {"-blendu", "-blendv", "-cc", "-clamp",
		"-texres", "-imfchan", "-bm"};
	static const int	nb_options = sizeof(options) / sizeof(char *);
	int					i;

	i = 0;
	while (i < nb_options)
	{
		if (!ft_strcmp(option, (char *)options[i]))
			return (true);
		i++;
	}
	return (false);
}

static bool	check_option_param_count(char **split, int *index)
{
	int					nb_p;
	int					i;
	bool				status;

	nb_p = 0;
	status = true;
	if (!ft_strcmp(split[0], "-o") || !ft_strcmp(split[0], "-t")
		|| !ft_strcmp(split[0], "-s"))
		nb_p = 3;
	else if (!ft_strcmp(split[0], "-mm"))
		nb_p = 2;
	else if (is_one_param_option(split[0]))
		nb_p = 1;
	i = 0;
	while (status && i < nb_p)
	{
		status = split[i] != NULL;
		i++;
	}
	*index += i + 1;
	return (status);
}

static int	parse_tex_option(char **split, t_texture *tex, int line_nb)
{
	bool	parse_result;
	int		i;

	parse_result = true;
	i = 1;
	while (parse_result && split[i] && split[i][0] == '-')
	{
		if (!ft_strcmp(split[i], "-s"))
			parse_result = parse_tex_vector(&split[i], &(tex->scale), "-s",
					line_nb);
		parse_result &= check_option_param_count(&split[i], &i);
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
	if (i == -1 || !split[i] || !split[i][0])
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
	{
		free(tex_path);
		return (0);
	}
	is_tex_loaded = load_texture(tex_path, tex, mlx);
	free(tex_path);
	if (!is_tex_loaded)
	{
		free(tex);
		return (0);
	}
	obj->tex = tex;
	return (1);
}
