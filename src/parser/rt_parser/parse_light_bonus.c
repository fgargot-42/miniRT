/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_light_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 18:12:11 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/22 01:09:43 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "parser_bonus.h"
#include "libft.h"

static int	parse_light_elements(char **line_split, t_object *obj, int line_nb)
{
	int	parse_result;
	int	nb_elements;

	nb_elements = get_str_array_length(line_split);
	parse_result = parse_vector(line_split[1], &(obj->position), "light",
			line_nb);
	parse_result &= parse_double(line_split[2], &(obj->props.intensity),
			"light", line_nb);
	if (nb_elements > 3 && !is_ignored(line_split[3]))  
		parse_result &= parse_vector(line_split[3], &(obj->color), "light",
				line_nb);
	return (parse_result);
}

t_object	*parse_light(char **line_split, t_parser_ctx *ctx, void *mlx)
{
	int			parse_result;
	t_object	*obj;

	(void)mlx;
	if (check_array_size(line_split, 3, "light", ctx->line_nb))
		return (NULL);
	obj = ft_calloc(1, sizeof(t_object));
	if (!obj)
	{
		print_parse_error("allocation failed", "light", ctx->line_nb);
		return (NULL);
	}
	obj->color = (t_vec3){{255, 255, 255}};
	parse_result = parse_light_elements(line_split, obj, ctx->line_nb);
	if (!parse_result)
	{
		free(obj);
		return (NULL);
	}
	obj->type = OBJ_LIGHT;
	return (obj);
}
