/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 20:22:03 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/04 22:22:36 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <mlx.h>
#include <stdlib.h>
#include "miniRT.h"

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
	mlx_mouse_move(data->mlx, data->win, WIDTH / 2, HEIGHT / 2);
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

void	print_sphere(t_sphere *s)
{
	printf("Object type: SPHERE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f)\n",
		s->center.x, s->center.y, s->center.z, s->radius);
	printf("--color: x=%f y=%f z=%f\n", s->color.x, s->color.y, s->color.z);
	printf("--specular: %f\n", s->specular);
	printf("--shininess: %f\n", s->shininess);
}

void	print_plane(t_plane *p)
{
	printf("Object type: PLANE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f\n",
		p->point.x, p->point.y, p->point.z);
	printf("--normal: x=%.2f y=%.2f z=%.2f\n",
		p->normal.x, p->normal.y, p->normal.z);
	printf("--color: x=%f y=%f z=%f (checkered: %s)\n", p->color.x, p->color.y, p->color.z,
		p->checker ? "true" : "false");
	printf("--specular: %f\n", p->specular);
	printf("--shininess: %f\n", p->shininess);
}

void	print_cylinder(t_cylinder *c)
{
	printf("Object type: CYLINDER\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (r=%.2f h=%.2f)\n",
		c->center.x, c->center.y, c->center.z, c->radius, c->height);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->axis.x, c->axis.y, c->axis.z);
	printf("--color: x=%f y=%f z=%f\n", c->color.x, c->color.y, c->color.z);
	printf("--specular: %f\n", c->specular);
	printf("--shininess: %f\n", c->shininess);
}

void	print_cone(t_cone *c)
{
	printf("Object type: CONE\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->center.x, c->center.y, c->center.z, c->angle, c->height, c->depth);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->axis.x, c->axis.y, c->axis.z);
	printf("--color: x=%f y=%f z=%f\n", c->color.x, c->color.y, c->color.z);
	printf("--specular: %f\n", c->specular);
	printf("--shininess: %f\n", c->shininess);
}

void	print_hyper(t_hyperboloid *c)
{
	printf("Object type: HYPERBOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f d=%.2f)\n",
		c->center.x, c->center.y, c->center.z, c->angle, c->height, c->depth);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->axis.x, c->axis.y, c->axis.z);
	printf("--color: x=%f y=%f z=%f\n", c->color.x, c->color.y, c->color.z);
	printf("--specular: %f\n", c->specular);
	printf("--shininess: %f\n", c->shininess);
}

void	print_para(t_paraboloid *c)
{
	printf("Object type: PARABOLOID\n");
	printf("--position: x=%.2f y=%.2f z=%.2f (a=%.2f h=%.2f)\n",
		c->center.x, c->center.y, c->center.z, c->angle, c->height);
	printf("--axis: x=%.2f y=%.2f z=%.2f\n",
		c->axis.x, c->axis.y, c->axis.z);
	printf("--color: x=%f y=%f z=%f\n", c->color.x, c->color.y, c->color.z);
	printf("--specular: %f\n", c->specular);
	printf("--shininess: %f\n", c->shininess);
}

void	print_obj_list(void *o)
{
	t_object *obj = (t_object *)o;

	if (obj->type == OBJ_SPHERE)
		print_sphere((t_sphere *)obj->object);
	if (obj->type == OBJ_PLANE)
		print_plane((t_plane *)obj->object);
	if (obj->type == OBJ_CYLINDER)
		print_cylinder((t_cylinder *)obj->object);
	if (obj->type == OBJ_CONE)
		print_cone((t_cone *)obj->object);
	if (obj->type == OBJ_HYPERBOLOID)
		print_hyper((t_hyperboloid *)obj->object);
	if (obj->type == OBJ_PARABOLOID)
		print_para((t_paraboloid *)obj->object);
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
	data.r_click_hold = 0;
	data.w_click_hold = 0;
	data.editor = NULL;
	data.nb_sliders=0;
	data.dragging_slider=-1;
	data.scene = malloc(sizeof(t_scene));
	if (!data.scene)
	{
		fprintf(stderr, "Failed to allocate scene\n");
		return (1);
	}
	init_scene(argv[1], data.scene);
	ft_lstiter(data.scene->objects, print_obj_list);
	init(&data);
	draw_single(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
