/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_debug_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 23:19:00 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/13 20:23:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "libft.h"

void	print_fps(t_data *data, double render_time_ms, t_vec2 pos);
void	print_camera_orientation(t_data *data, t_vec2 pos);
void	print_vector(t_data *data, char *name, t_vec3 vec, t_vec2 pos);
void	print_int_value(t_data *data, char *label, int value, t_vec2 pos);

static void	print_nb_threads(t_data *data, t_vec2 pos)
{
	char	*nb_threads_str;
	char	*print_tmp;
	char	*print_str;

	if (data->nb_threads <= 1)
	{
		mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
			(mlx_color){.rgba = MLX_WHITE}, "Single-thread mode");
		return ;
	}
	nb_threads_str = ft_itoa(data->nb_threads);
	if (!nb_threads_str)
		return ;
	print_tmp = ft_strjoin("Multi-thread mode using ", nb_threads_str);
	free(nb_threads_str);
	if (!print_tmp)
		return ;
	print_str = ft_strjoin(print_tmp, " threads");
	free(print_tmp);
	if (!print_str)
		return ;
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, print_str);
	free(print_str);
}

static void	print_toggle(t_data *data, char *label, bool value, t_vec2 pos)
{
	static const char	*on_off[] = {"off", "on"};
	char				*print_str;
	char				*print_label;

	print_label = ft_strjoin(label, ": ");
	if (!print_label)
		return ;
	print_str = ft_strjoin(print_label, on_off[value]);
	free(print_label);
	if (!print_str)
		return ;
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, print_str);
	free(print_str);
}

void	add_debug(t_data *data, double render_time_ms)
{
	print_fps(data, render_time_ms, (t_vec2){{10, 20}});
	print_nb_threads(data, (t_vec2){{10, 40}});
	print_vector(data, "POS", data->scene->cam->position, (t_vec2){{10, 60}});
	print_camera_orientation(data, (t_vec2){{10, 80}});
	print_int_value(data, "BVH DISPLAY LEVEL", data->scene->bvh_display_level,
		(t_vec2){{10, 100}});
	print_toggle(data, "transparency (T)", data->scene->transparency,
		(t_vec2){{10, 120}});
	print_toggle(data, "specular (G)", data->scene->specular,
		(t_vec2){{10, 140}});
	print_toggle(data, "bump (B)", data->scene->bump, (t_vec2){{10, 160}});
	print_toggle(data, "anti-aliasing (Y)", data->scene->anti_aliasing,
		(t_vec2){{10, 180}});
}
