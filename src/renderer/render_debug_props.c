/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_debug_props.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/01 20:36:04 by fgargot           #+#    #+#             */
/*   Updated: 2026/08/13 19:29:44 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT_bonus.h"
#include "libft.h"

void	print_vector(t_data *data, char *name, t_vec3 vec, t_vec2 pos)
{
	char	*str_arr[5];
	char	*print_str;
	int		i;

	str_arr[0] = ft_strjoin(name, ":");
	str_arr[1] = ft_dtoa(vec.x, 2);
	str_arr[2] = ft_dtoa(vec.y, 2);
	str_arr[3] = ft_dtoa(vec.z, 2);
	str_arr[4] = NULL;
	if (str_arr[0] && str_arr[1] && str_arr[2] && str_arr[3])
		print_str = ft_strjoin_array((const char **)str_arr, " ");
	i = 0;
	while (i < 4)
	{
		if (str_arr[i])
			free(str_arr[i]);
		i++;
	}
	if (!print_str)
		return ;
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, print_str);
	free(print_str);
}

void	print_int_value(t_data *data, char *label, int value, t_vec2 pos)
{
	char	*value_str;
	char	*print_tmp;
	char	*print_str;

	print_tmp = ft_strjoin(label, ": ");
	if (!print_tmp)
		return ;
	value_str = ft_itoa(value);
	print_str = ft_strjoin(print_tmp, value_str);
	free(value_str);
	free(print_tmp);
	if (!print_str)
		return ;
	mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
		(mlx_color){.rgba = MLX_WHITE}, print_str);
	free(print_str);
}

void	print_fps(t_data *data, double render_time_ms, t_vec2 pos)
{
	char	*print_fps[6];
	char	*print_str;
	double	fps;

	fps = 1000.0 / render_time_ms;
	print_str = NULL;
	print_fps[0] = "Frame rendered in ";
	print_fps[1] = ft_dtoa(render_time_ms, 2);
	print_fps[2] = " ms (";
	print_fps[3] = ft_dtoa(fps, 2);
	print_fps[4] = " FPS)";
	print_fps[5] = NULL;
	if (print_fps[1] && print_fps[3])
	{
		print_str = ft_strjoin_array((const char **)print_fps, "");
		if (print_str)
			mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
				(mlx_color){.rgba = MLX_WHITE}, print_str);
	}
	if (print_fps[1])
		free(print_fps[1]);
	if (print_fps[3])
		free(print_fps[3]);
	if (print_str)
		free(print_str);
}

void	print_camera_orientation(t_data *data, t_vec2 pos)
{
	char	*print_cam_o[5];
	char	*print_str;

	print_str = NULL;
	print_cam_o[0] = "YAW: ";
	print_cam_o[1] = ft_dtoa(data->scene->cam->rotation.y, 2);
	print_cam_o[2] = "  PITCH:";
	print_cam_o[3] = ft_dtoa(data->scene->cam->rotation.x, 2);
	print_cam_o[4] = NULL;
	if (print_cam_o[1] && print_cam_o[3])
	{
		print_str = ft_strjoin_array((const char **)print_cam_o, "");
		if (print_str)
			mlx_string_put(data->mlx, data->win, (int)pos.x, (int)pos.y,
				(mlx_color){.rgba = MLX_WHITE}, print_str);
	}
	if (print_cam_o[1])
		free(print_cam_o[1]);
	if (print_cam_o[3])
		free(print_cam_o[3]);
	if (print_str)
		free(print_str);
}
