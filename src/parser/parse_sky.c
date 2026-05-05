/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sky.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/05 22:04:25 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

t_object	*parse_sky(char **line_split, int line_nb)
{
	t_object	*obj;
	int			parse_result;

	obj = NULL;
	if (check_array_size(line_split, 2, "sky", line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "sky", line_nb);
		return (NULL);
	}
	parse_result = parse_vector(line_split[1], &(obj->color), "sky", line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->type = OBJ_SKY;
	return (obj);
}
