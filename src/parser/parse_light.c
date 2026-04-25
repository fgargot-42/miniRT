/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/24 23:59:53 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_light_elements(char **line_split, t_light *light, int line_nb)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(light->position), "light",
			line_nb);
	parse_result &= parse_double(line_split[2], &(light->intensity), "light",
			line_nb);
	parse_result &= parse_vector(line_split[3], &(light->color), "light",
			line_nb);
	return (parse_result);
}

t_object	*parse_light(char **line_split, int line_nb)
{
	int			parse_result;
	t_light		*light;
	t_object	*obj;

	obj = NULL;
	if (check_array_size(line_split, 4, "light", line_nb))
		return (NULL);
	light = malloc(sizeof(t_light));
	if (!light)
	{
		print_parse_error("allocation failed", "light", line_nb);
		return (NULL);
	}
	parse_result = parse_light_elements(line_split, light, line_nb);
	if (parse_result)
		obj = create_object(light, OBJ_LIGHT);
	if (!obj)
		free(light);
	return (obj);
}
