/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_ambient.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 23:20:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

static int	parse_ambient_elements(char **line_split, t_object *obj,
	int line_nb)
{
	int	p_res;

	p_res = parse_vector(line_split[2], &obj->color, "ambient", line_nb);
	p_res &= parse_double(line_split[1], &obj->props.intensity, "ambient",
			line_nb);
	return (p_res);
}

t_object	*parse_ambient(char **line_split, int line_nb)
{
	int			parse_result;
	t_object	*obj;

	obj = NULL;
	parse_result = 1;
	if (check_array_size(line_split, 3, "ambient", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "ambient", line_nb);
		return (NULL);
	}
	parse_result = parse_ambient_elements(line_split, obj, line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->type = OBJ_AMBIENT;
	return (obj);
}
