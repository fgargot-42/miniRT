/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_texture_utils_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/12 01:13:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/12 01:49:07 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "veclib.h"
#include "parser_bonus.h"
#include <stdbool.h>

char	*get_tex_path(char *rt_path, char *tex_path)
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

bool	parse_tex_vector(char **split, t_vec3 *vec, char *param,
	int line_nb)
{
	bool	parse_result;

	if (!split[1] || !split[2] || !split[3])
		return (false);
	parse_result = parse_double(split[1], &vec->x, param, line_nb);
	parse_result &= parse_double(split[2], &vec->y, param, line_nb);
	parse_result &= parse_double(split[3], &vec->z, param, line_nb);
	return (parse_result);
}
