/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/13 20:07:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_light(char **line_split, t_scene *scene, int line_nb)
{
	int		parse_result;
	t_light	*light;

	if (check_array_size(line_split, 4, "light", line_nb))
		return (0);
	light = malloc(sizeof(t_light));
	if (!light)
	{
		print_parse_error("allocation failed", "light", line_nb);
		return (0);
	}
	parse_result = parse_vector(line_split[1], &(light->position), "light",
			line_nb);
	parse_result &= parse_double(line_split[2], &(light->intensity), "light",
			line_nb);
	parse_result &= parse_vector(line_split[3], &(light->color), "light",
			line_nb);
	ft_lstadd_back(&scene->lights, ft_lstnew(light));
	return (parse_result);
}
