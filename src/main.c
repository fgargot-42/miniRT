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



mlx_color	vec3_to_color(t_vec3 v)
{
	return ((mlx_color){
		.r = (int)v.x,
		.g = (int)v.y,
		.b = (int)v.z,
		.a = 255
	});
}

void	init(t_data *data)
{
	mlx_window_create_info	info;

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

void	init_scene(t_scene *scene)
{
	t_sphere	*s;
	t_plane		*p;
	t_cylinder	*c;

	ft_bzero(scene, sizeof(t_scene));

	// Camera
	scene->cam.position = (t_vec3){0, 2, -5};
	scene->cam.direction =  (t_vec3){0, 0, 1};
	scene->cam.fov = 80;

	// Warm key light from top left
	scene->light.position = (t_vec3){-4, 8, 2};
	scene->light.intensity = 0.8;
	scene->light.color = (t_vec3){255, 240, 200};

	// Soft ambient
	scene->ambient = (t_vec3){15, 15, 25};

	// Checkered floor
	p = malloc(sizeof(t_plane));
	p->point = (t_vec3){0, -1.5, 0};
	p->normal = (t_vec3){0, 1, 0};
	p->color = (t_vec3){200, 200, 200};
	p->checker = 1;
	ft_lstadd_back(&scene->planes, ft_lstnew(p));

	// Big center sphere - matte white
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){0, 0, 6};
	s->radius = 1.5;
	s->color = (t_vec3){230, 230, 230};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Left sphere - deep blue
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){-3, -0.5, 5};
	s->radius = 1.0;
	s->color = (t_vec3){50, 80, 200};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Right sphere - coral red
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){3, -0.5, 5};
	s->radius = 1.0;
	s->color = (t_vec3){220, 80, 60};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));
	// Small sphere on top of center - gold
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){0, 2.2, 6};
	s->radius = 0.5;
	s->color = (t_vec3){255, 200, 50};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Tiny sphere front left - mint green
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){-1.5, -1.0, 3};
	s->radius = 0.35;
	s->color = (t_vec3){100, 220, 150};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Tiny sphere front right - purple
	s = malloc(sizeof(t_sphere));
	s->center = (t_vec3){1.5, -1.0, 3};
	s->radius = 0.35;
	s->color = (t_vec3){180, 80, 220};
	ft_lstadd_back(&scene->spheres, ft_lstnew(s));

	// Cylinder along y (green)
	c = malloc(sizeof(t_cylinder));
	c->center = (t_vec3){2, 1, 8};
	c->axis = vec_normalize((t_vec3){0, 1, 0});
	c->radius = 2;
	c->height = 4;
	c->color = (t_vec3){0, 153, 0};
	ft_lstadd_back(&scene->cylinder, ft_lstnew(c));

	// Cylinder along x (red)
	c = malloc(sizeof(t_cylinder));
	c->center = (t_vec3){2, 2, 4};
	c->axis = vec_normalize((t_vec3){1, 0, 0});
	c->radius = 1;
	c->height = 4;
	c->color = (t_vec3){204, 0, 0};
	ft_lstadd_back(&scene->cylinder, ft_lstnew(c));

	// Cylinder along z (blue)
	c = malloc(sizeof(t_cylinder));
	c->center = (t_vec3){-2, 1, 4};
	c->axis = vec_normalize((t_vec3){0, 1, 1});
	c->radius = 1;
	c->height = 4;
	c->color = (t_vec3){0, 0, 204};
	ft_lstadd_back(&scene->cylinder, ft_lstnew(c));
}


#include <stdio.h>

#include <stdio.h>
#include <sys/time.h>
#include "mlx.h"

static double	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec + tv.tv_usec / 1000000.0);
}

void	add_debug(t_data *data)
{
	static double	last_time = 0;
	static int		frame_count = 0;
	static double	fps = 0;

	char		buf[128];
	int			y;
	mlx_color	white;

	white = vec3_to_color((t_vec3){255, 255, 255});
	y = 20;

	frame_count++;
	double now = get_time();
	if (now - last_time >= 1.0)
	{
		fps = frame_count / (now - last_time);
		frame_count = 0;
		last_time = now;
	}

	sprintf(buf, "FPS: %.1f", fps);
	mlx_string_put(data->mlx, data->win, 10, y, white, buf);
	y += 20;

	sprintf(buf, "Render Scale: %d", data->render_scale);
	mlx_string_put(data->mlx, data->win, 10, y, white, buf);
	y += 20;

	sprintf(buf, "POS: %.2f %.2f %.2f",
		data->scene->cam.position.x,
		data->scene->cam.position.y,
		data->scene->cam.position.z);
	mlx_string_put(data->mlx, data->win, 10, y, white, buf);
	y += 20;

	sprintf(buf, "DIR: %.2f %.2f %.2f",
		data->scene->cam.direction.x,
		data->scene->cam.direction.y,
		data->scene->cam.direction.z);
	mlx_string_put(data->mlx, data->win, 10, y, white, buf);
	y += 20;

	sprintf(buf, "YAW: %.2f  PITCH: %.2f",
		data->scene->cam.yaw,
		data->scene->cam.pitch);
	mlx_string_put(data->mlx, data->win, 10, y, white, buf);
}

void draw(t_data *data)
{
    t_ray        r;
    t_hit_record hc;
    t_scene      *scene = data->scene;
    int          x, y;
    int          i, bx, by;
    mlx_color    color;

    x = 0;
    while (x < WIDTH)
    {
        y = 0;
        while (y < HEIGHT)
        {
            r = camera_ray(&scene->cam,
                           x + data->render_scale / 2,
                           y + data->render_scale / 2);

            if (hit_scene(scene, &r, T_MAX, &hc))
                color = vec3_to_color(shade(&hc, scene));
            else
                color = vec3_to_color((t_vec3){0, 0, 0});

            i = 0;
            while (i < data->render_scale * data->render_scale)
            {
                bx = i % data->render_scale;
                by = i / data->render_scale;
                if ((x + bx) < WIDTH && (y + by) < HEIGHT)
                    mlx_set_image_pixel(data->mlx, data->img,
                                        x + bx, y + by, color);
                i++;
            }

            y += data->render_scale;
        }
        x += data->render_scale;
    }

	mlx_clear_window(data->mlx, data->win, vec3_to_color((t_vec3){0,0,0}));
    mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
	add_debug(data);
}

static void	destroy_all(t_data *data)
{
	mlx_destroy_image(data->mlx, data->img);
	mlx_destroy_window(data->mlx, data->win);
	mlx_destroy_context(data->mlx);
}




void rotate_camera(t_vec3 *direction, double *yaw, double *pitch,
                   double mouse_dx, double mouse_dy, double sensitivity)
{
    *yaw   += mouse_dx * sensitivity;
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
        data->render_scale = 8;

        rotate_camera(&data->scene->cam.direction,
                      &data->scene->cam.yaw,
                      &data->scene->cam.pitch,
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

int	main(void)
{
	t_data	data;
	data.render_scale = 1;
	data.scene = malloc(sizeof(t_scene));
    if (!data.scene)
    {
        fprintf(stderr, "Failed to allocate scene\n");
        return 1;
    }
	init_scene(data.scene);
	init(&data);
	mlx_mouse_hide(data.mlx);
	draw(&data);
	attach_hooks(&data);
	mlx_add_loop_hook(data.mlx, mouse_loop, &data);
	mlx_loop(data.mlx);
	destroy_all(&data);
	return (0);
}
