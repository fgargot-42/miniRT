/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:03:32 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

int	parse_light_elements(char **line_split, t_object *obj, int line_nb)
{
	int	parse_result;

	parse_result = parse_vector(line_split[1], &(obj->position), "light",
			line_nb);
	parse_result &= parse_double(line_split[2], &(obj->props.intensity),
			"light", line_nb);
	parse_result &= parse_vector(line_split[3], &(obj->color), "light",
			line_nb);
	return (parse_result);
}

t_object	*parse_light(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	if (check_array_size(line_split, 4, "light", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "light", line_nb);
		return (NULL);
	}
	parse_result = parse_light_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->type = OBJ_LIGHT;
	return (obj);
}
