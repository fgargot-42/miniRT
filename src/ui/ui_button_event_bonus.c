/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_button_event_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/08 01:30:04 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/08 02:49:39 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"

void	select_next_light(void *data)
{
	t_data	*d;

	d = (t_data *)data;
	d->ui.selected_light++;
	if (d->ui.selected_light == (int)d->scene->lights.len)
		d->ui.selected_light = 0;
	setup_light_sliders(d);
	draw_light_editor(d);
}

void	select_prev_light(void *data)
{
	t_data	*d;

	d = (t_data *)data;
	d->ui.selected_light--;
	if (d->ui.selected_light == -1)
		d->ui.selected_light = (int)d->scene->lights.len - 1;
	setup_light_sliders(d);
	draw_light_editor(d);
}

void	setup_button_event(t_button *button, char *label, t_vec2 pos,
		void (*event)(void *))
{
	button->label = label;
	button->pos = pos;
	button->event = event;
	button->size.x = 20;
	button->size.y = 20;
}
