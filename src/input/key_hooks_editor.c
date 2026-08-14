/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks_editor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 21:54:15 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/14 22:08:03 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	keydown_hook_editor(int scancode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (scancode == SDL_SCANCODE_LSHIFT)
		data->ui.is_lshift_enabled = true;
}

static void	keyup_hook_editor(int scancode, void *param)
{
	t_data	*data;

	data = (t_data *)param;
	if (scancode == SDL_SCANCODE_LSHIFT)
		data->ui.is_lshift_enabled = false;
}

void	editor_attach_hooks(t_data *data)
{
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEDOWN, editor_mouse_down,
		data);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEUP, editor_mouse_up, data);
	mlx_on_event(data->mlx, data->editor, MLX_KEYDOWN, keydown_hook_editor,
		data);
	mlx_on_event(data->mlx, data->editor, MLX_KEYUP, keyup_hook_editor, data);
}
