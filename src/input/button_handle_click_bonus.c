/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button_handle_click_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 02:32:08 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/08 02:43:59 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

void	handle_button_click(t_data *data, t_button button, int mx, int my)
{
	if (!button.event)
		return ;
	if (mx >= button.pos.x && mx <= button.pos.x + button.size.x
		&& my >= button.pos.y && my <= button.pos.y + button.size.y)
		button.event((void *)data);
}
