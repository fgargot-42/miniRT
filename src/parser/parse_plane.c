/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 20:08:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_plane(char **line_split, t_scene *scene, int line_nb)
{
	int		parse_result;
	t_plane	*pl;

	if (check_array_size(line_split, 5, "plane", line_nb))
		return (0);
	pl = malloc(sizeof(t_plane));
	if (!pl)
		print_parse_error("allocation failed", "plane", line_nb);
	if (!pl)
		return (0);
	parse_result = parse_vector(line_split[1], &(pl->point), "plane", line_nb);
	parse_result &= parse_vector(line_split[2], &(pl->normal), "plane",
			line_nb);
	parse_result &= parse_vector(line_split[3], &(pl->color), "plane", line_nb);
	pl->checker = ft_atoi(line_split[4]);
	pl->specular = 0;
	pl->shininess = 1;
	if (line_split[5] && ft_strlen(line_split[5]))
		parse_result &= parse_double(line_split[5], &(pl->specular),
				"plane", line_nb);
	if (line_split[5] && line_split[6] && ft_strlen(line_split[6]))
		parse_result &= parse_double(line_split[6], &(pl->shininess),
				"plane", line_nb);
	ft_lstadd_back(&scene->planes, ft_lstnew(pl));
	return (parse_result);
}
