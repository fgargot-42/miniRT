/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_plane.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 00:00:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_plane_elements(char **line_split, t_plane *pl, int line_nb)
{
	int	parse_result;

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
	if (parse_result)
		pl->normal = vec3_normalize(pl->normal);
	return (parse_result);
}

t_object	*parse_plane(char **line_split, int line_nb)
{
	int			parse_result;
	t_plane		*pl;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 5, "plane", line_nb))
		return (0);
	pl = malloc(sizeof(t_plane));
	if (!pl)
		print_parse_error("allocation failed", "plane", line_nb);
	if (!pl)
		return (0);
	parse_result = parse_plane_elements(line_split, pl, line_nb);
	if (!parse_result)
		free(pl);
	if (parse_result)
		obj = create_object(pl, OBJ_PLANE);
	if (!obj)
		free(pl);
	return (obj);
}
