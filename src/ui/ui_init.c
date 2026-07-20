/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/20 19:04:24 by mabarrer          #+#    #+#             */
/*   Updated: 2026/07/20 19:04:35 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

void	init_editor(t_data *data)
{
	mlx_window_create_info	info;

	if (data->editor)
		return ;
	info = (mlx_window_create_info){
		.title = "inspector",
		.width = EDITOR_W,
		.height = EDITOR_H,
	};
	data->editor = mlx_new_window(data->mlx, &info);
	if (!data->editor)
		exit(1);
	mlx_set_window_size(data->mlx, data->editor, EDITOR_W, EDITOR_H);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEDOWN, editor_mouse_down,
		data);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEUP, editor_mouse_up, data);
}
