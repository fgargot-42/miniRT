/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 20:22:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/20 23:08:34 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

#define CENTER_X (WIDTH / 2)
#define CENTER_Y (HEIGHT / 2)

void	clear_image(t_data *fdf)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			mlx_set_image_pixel(fdf->mlx, fdf->img, x, y,
				(mlx_color){.rgba = 0x000000FF});
			x++;
		}
		y++;
	}
}

void	init(t_data *data)
{
	mlx_window_create_info	info;

	data->nb_threads = NB_THREADS;
	data->th_nb = 0;
	data->mlx = mlx_init();
	if (!data->mlx)
		exit(1);
	info = (mlx_window_create_info)
	{
		.title = "miniRT",
		.width = WIDTH,
		.height = HEIGHT,
	};
	data->win = mlx_new_window(data->mlx, &info);
	if (!data->win)
		exit(1);
	mlx_mouse_get_pos(data->mlx, &data->last_mouse_x, &data->last_mouse_y);
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		exit(1);

}

static void	destroy_all(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_context(data->mlx);
	free_scene(data->scene);
}

void rotate_camera(t_vec3 *direction, double *yaw, double *pitch,
                   double mouse_dx, double mouse_dy, double sensitivity)
{
    *yaw   -= mouse_dx * sensitivity;
    *pitch -= mouse_dy * sensitivity;

    if (*pitch > 89.0) *pitch = 89.0;
    if (*pitch < -89.0) *pitch = -89.0;

    double yaw_rad   = *yaw * (M_PI / 180.0);
    double pitch_rad = *pitch * (M_PI / 180.0);

    double cos_pitch = cos(pitch_rad);
    double sin_pitch = sin(pitch_rad);
    double cos_yaw   = cos(yaw_rad);
    double sin_yaw   = sin(yaw_rad);

    direction->x = cos_yaw * cos_pitch;
    direction->y = sin_pitch;
    direction->z = sin_yaw * cos_pitch;

    *direction = vec_normalize(*direction);
}

void mouse_loop(void *param)
{
    static int is_moving = 0;

    t_data *data = param;
    int x, y;
    int dx, dy;

    mlx_mouse_get_pos(data->mlx, &x, &y);

    dx = x - data->last_mouse_x;
    dy = y - data->last_mouse_y;

    if (!dx && !dy && !is_moving)
        return;

    if (dx || dy)
    {
        is_moving = 1;
		if (NB_THREADS <= 1)
			data->render_scale = 8;

        rotate_camera(&data->scene->cam->direction,
                      &data->scene->cam->yaw,
                      &data->scene->cam->pitch,
                      (double)dx,
                      (double)dy,
                      CAMERA_SENS);
		draw(data);

        mlx_mouse_move(data->mlx, data->win, CENTER_X, CENTER_Y);
        data->last_mouse_x = CENTER_X;
        data->last_mouse_y = CENTER_Y;
    }
    else
    {
        is_moving = 0;
        data->render_scale = 1; 
        draw(data);
    }
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
	{
		ft_putstr_fd("Error\n1 argument expected\n./miniRT <scene.rt>\n", 2);
		return (1);
	}
	data.render_scale = 1;
	data.scene = malloc(sizeof(t_scene));
    if (!data.scene)
    {
        fprintf(stderr, "Failed to allocate scene\n");
        return (1);
    }
	init_scene(argv[1], data.scene);
	init(&data);
	mlx_mouse_hide(data.mlx);
	draw_single(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
