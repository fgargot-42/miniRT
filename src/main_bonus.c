/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 20:22:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/06 20:49:12 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "mlx.h"
#include <stdlib.h>

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		ft_putstr_fd("Error\n1 argument expected\n./miniRT <scene.rt>\n", 2);
		return (1);
	}
	ft_bzero(&data, sizeof(t_data));
	data.scene = ft_calloc(1, sizeof(t_scene));
	if (!data.scene)
	{
		ft_putstr_fd("Failed to allocate scene\n", 2);
		return (1);
	}
	init_display(argv[1], &data);
	draw(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	free_scene(data.scene, data.mlx);
	destroy_display(&data);
	return (0);
}
