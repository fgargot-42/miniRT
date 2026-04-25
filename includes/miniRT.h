/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:43:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/04/25 20:07:57 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H
# include "veclib.h"
# include "mlx.h"
# include "math.h"
# include "SDL2/SDL_scancode.h"
# define T_MIN 0.001
# define T_MAX 100

# include "libft.h"

# define WIDTH 800
# define HEIGHT 600
# define CAMERA_SENS 0.35
# define MOVE_STEP 0.5

# define NB_THREADS 16

# define DEBUG 1

# define MLX_WHITE 0xFFFFFFFF

typedef struct s_ray
{
	t_vec3		origin;
	t_vec3		direction;
}	t_ray;

// P(t) = O + t * D
// t distance sur le ray
// O origin
// D direction

typedef struct s_ambient
{
	double	intensity;
	t_vec3	color;
}	t_ambient;

typedef struct s_light
{
	t_vec3	position;
	double	intensity;
	t_vec3	color;
}	t_light;

typedef struct s_sphere
{
	t_vec3	center;
	double	radius;
	t_vec3	color;
	double	specular;
	double	shininess;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal; // faut normaliser
	t_vec3	color;
	int		checker;
	double	specular;
	double	shininess;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	double	**transform_axis;
	double	radius;
	double	height;
	t_vec3	color;
	double	specular;
	double	shininess;
}	t_cylinder;

typedef struct s_cone
{
	t_vec3	center;
	t_vec3	axis;
	double	**transform_axis;
	double	angle;
	double	tan_angle;
	double	height;
	double	depth;
	t_vec3	color;
	double	specular;
	double	shininess;
}	t_cone;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	direction;
	double	fov;
	double	pitch;
	double	yaw;
}	t_camera;

typedef enum	e_obj_type
{
	OBJ_AMBIENT,
	OBJ_CAMERA,
	OBJ_SKY,
	OBJ_LIGHT,
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE
}	t_obj_type;

typedef struct	s_object
{
	void		*object;
	t_obj_type	type;
}	t_object;

typedef struct s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		color;
	t_object	*object;
	double		specular;
	double		shininess;
}	t_hit_record;

typedef struct s_hit_fn
{
	t_obj_type 	type;
	int			(*hit_fn)(void *, t_ray *, double, t_hit_record *);
}	t_hit_fn;

typedef struct s_scene
{
	t_list		*objects;
	t_list		*lights;
	t_ambient	*ambient;
	t_vec3		*sky;
	t_camera	*cam;
	t_object	*selected;
}	t_scene;

typedef struct s_data
{
	mlx_context	mlx;
	mlx_window	win;
	mlx_image	img;
	t_scene		*scene;
	int			r_click_hold;
	int			w_click_hold;
	int			last_mouse_x;
	int			last_mouse_y;
	int			render_scale;
	int			th_nb;
	int			nb_threads;
}	t_data;

// OBJECTS

t_object			*create_object(void *object, t_obj_type type);

// PARSING

typedef t_object	*(*t_parser_func)(char **, int);
int					parse_scene(char *file, t_scene *scene);
t_object			*parse_ambient(char **line_split, int line_nb);
t_object			*parse_camera(char **line_split, int line_nb);
t_object			*parse_sky(char **line_split, int line_nb);
t_object			*parse_light(char **line_split, int line_nb);
t_object			*parse_sphere(char **line_split, int line_nb);
t_object			*parse_plane(char **line_split, int line_nb);
t_object			*parse_cylinder(char **line_split, int line_nb);
t_object			*parse_cone(char **line_split, int line_nb);

void		print_parse_error(char *message, char *element, int line_nb);
void		clear_gnl(int fd, char *line);
int			check_array_size(char **array, int expected, char *object,
				int line_nb);
int			parse_vector(char *param, t_vec3 *v_res, char *object, int line_nb);
int			parse_double(char *param, double *res, char *object, int line_nb);

// SCENE
void		init_scene(char *file, t_scene *scene);
void		free_scene(t_scene *scene);

// DRAWER

void		draw(t_data *data);
void		draw_single(t_data *data);
void		add_debug(t_data *data);
mlx_color	vec3_to_color(t_vec3 v);

//src/hooks.c
void		attach_hooks(t_data *data);

void		mouse_down_hook(int mouse_event, void *param);
void		mouse_up_hook(int mouse_event, void *param);
void		mouse_wheel_hook(int mouse_event, void *param);

//src/camera.c
t_ray		camera_ray(t_camera *cam, int x, int y);
void		mouse_loop(void *param);

//src/hit.c
t_vec3		face_normal(t_ray *ray, t_vec3 inverted);
int			hit_scene(t_scene *scene, t_ray *ray, double t_max,
				t_hit_record *rec);

int			hit_sphere(void *sphere, t_ray *ray, double t_max, t_hit_record *rec);
int			hit_plane(void *plane, t_ray *ray, double t_max, t_hit_record *rec);
int			hit_cylinder(void *cyl, t_ray *ray, double t_max, t_hit_record *rec);
int			hit_cone(void *cone, t_ray *ray, double t_max, t_hit_record *rec);

//src/ray.c
t_vec3		ray_at(t_ray ray, double t);

//lighting.c
t_vec3		shade(t_hit_record *rec, t_scene *scene, t_ray *ray);

// UTILS

int			get_polynom2_roots(double *roots, double a, double b, double c);
int			open_file_read(char *file);
void		free_str_array(char **array);

#endif
