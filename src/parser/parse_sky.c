/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_sky.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 00:05:21 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "libft.h"

t_object	*parse_sky(char **line_split, int line_nb)
{
	t_vec3		*sky;
	t_object	*obj;
	int			parse_result;

	obj = NULL;
	if (check_array_size(line_split, 2, "sky", line_nb))
		return (NULL);
	sky = malloc(sizeof(t_vec3));
	if (!sky)
	{
		print_parse_error("allocation failed", "sky", line_nb);
		return (NULL);
	}
	parse_result = parse_vector(line_split[1], sky, "sky", line_nb);
	if (parse_result)
		obj = create_object(sky, OBJ_SKY);
	if (!obj)
		free(sky);
	return (obj);
}
