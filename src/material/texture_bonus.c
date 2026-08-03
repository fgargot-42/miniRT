/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 18:30:20 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/03 19:29:31 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "material.h"
#include "mlx.h"
#include "veclib.h"
#include <stdio.h>
#include <stdlib.h>

t_texture	*new_texture(void)
{
	t_texture	*tex;

	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->scale = (t_vec3){{1.0, 1.0, 1.0}};
	return (tex);
}

bool	load_texture(char *path, t_texture *tex, mlx_context mlx)
{
	if (!tex)
		return (false);
	tex->data = mlx_new_image_from_file(mlx, path, &tex->width, &tex->height);
	if (!tex->data)
	{
		printf("Failed to load image\n");
		return (false);
	}
	tex->mlx = mlx;
	mlx_get_image_pixel(tex->mlx, tex->data, 0, 0);
	printf("Image loaded: %s (%i x %i) at address %p\n", path,
		tex->width, tex->height, tex->data);
	return (true);
}

void	destroy_texture(t_texture **t)
{
	if (!*t)
		return ;
	if ((*t)->data)
		mlx_destroy_image((*t)->mlx, (*t)->data);
	free(*t);
	*t = NULL;
}
