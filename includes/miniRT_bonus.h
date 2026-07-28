/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT_bonus.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabarrer <mabarrer@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:43:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/07/28 17:04:01 by mabarrer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_BONUS_H
# define MINIRT_BONUS_H

# define T_MIN 0.001
# define T_MAX 5000
# define WIDTH 1280
# define HEIGHT 720
# define CAMERA_SENS 0.35
# define MOVE_STEP 0.5
# define NB_THREADS 16
# define BVH_DEPTH 24
# define DEBUG 1
# define BVH_VIEW 1
# define SAH_BINS 64
# define MLX_WHITE 0xFFFFFFFF

# define MAX_SLIDERS    32

# include "mlx.h"
# include "veclib.h"
# include "object_bonus.h"
# include "bvh_bonus.h"
# include "ui.h"
# include <math.h>
# include "SDL2/SDL_scancode.h"
# include "libft.h"
# include "material.h"

typedef struct s_slider
{
	int			y;
	double		*value;
	double		min;
	double		max;
	const char	*label;
	mlx_color	col;
	bool		affects_bvh;
}	t_slider;

typedef struct s_scene
{
	t_array		objects;
	t_array		bvh_objects;
	t_object	*ambient;
	t_object	*cam;
	t_object	*selected;
	t_bvh		*bvh;
	int			bvh_display_level;
	t_array		lights;
	t_object	*sky;
	t_texture	*skybox;
	t_array		mat;
	bool		transparency;
}	t_scene;

typedef struct s_data
{
	mlx_context	mlx;
	mlx_window	win;
	mlx_window	editor;
	mlx_image	img;
	t_scene		*scene;
	bool		r_click_hold;
	bool		w_click_hold;
	int			last_mouse_x;
	int			last_mouse_y;
	int			render_scale;
	t_slider	sliders[MAX_SLIDERS];
	int			nb_sliders;
	int			dragging_slider;
	int			th_nb;
	int			nb_threads;
}	t_data;

// DISPLAY

void				init_display(char *rt_file, t_data *data);
void				destroy_display(t_data *data);

// DEBUG

void				print_sky(t_object *sky);
void				print_object(void *o);
void				print_bvh_tree(t_bvh *bvh, int depth);

// SCENE
void				init_scene(char *file, t_data *data);
void				free_scene(t_scene *scene, mlx_context mlx);
void				free_object(void *object);
void				check_scene_mandatory_object(void *obj, char *obj_str,
						t_scene *scene, mlx_context mlx);

// DRAWER

void				draw(t_data *data);
void				draw_single(t_data *data);
void				add_debug(t_data *data);
void				open_inspector(t_data *data, t_hit_record hc,
						double mouse_x, double mouse_y);
void				print_hit_info_debug(t_hit_record hc, t_scene *scene,
						t_ray *ray, t_vec2 mouse_pos);
mlx_color			vec3_to_color(t_vec3 v);
void				add_debug(t_data *data);
t_vec3				draw_skybox(t_scene *scene, t_ray r);
t_vec3				rt_cast(t_scene *scene, t_ray *r, int depth);

// HOOKS
void				attach_hooks(t_data *data);

void				mouse_down_hook(int mouse_event, void *param);
void				mouse_up_hook(int mouse_event, void *param);
void				mouse_wheel_hook(int mouse_event, void *param);

// CAMERA
t_ray				camera_ray(t_object *cam, int x, int y);
void				mouse_loop(void *param);

// LIGHTING
t_vec3				shade(t_hit_record *rec, t_scene *scene, t_ray *ray);
double				smoothstep(double min, double max, double value);
void				apply_normal(t_ray *ray, t_hit_record *rec);
void				ray_bounce(t_scene *scene, t_hit_record *rec, t_ray *ray);

// UTILS

int					get_polynom2_roots(double *roots, double a, double b,
						double c);
int					open_file_read(char *file, char *extension);
char				*get_directory_path(char *filepath);
void				free_str_array(char **array);
size_t				get_str_array_length(char **array);
void				free_array(void **array);

t_vec3				srgb_to_linear(t_vec3 srgb);
t_vec3				linear_to_srgb(t_vec3 linear_rgb);

double				get_time(void);

// UI
void				setup_sliders(t_data *data);
void				draw_editor(t_data *data, double mouse_x, double mouse_y);
void				draw_light_editor(t_data *d);
void				editor_mouse_down(int event, void *param);
void				editor_mouse_up(int event, void *param);
void				editor_loop(void *param);

void				fill_rect(t_data *data, t_vec2 pos, t_vec2 size,
						mlx_color col);

void				draw_hline(t_data *data, void *win, int y);

void				put_row(t_data *data, t_row_info info,
						const char *fmt, ...);
void				put_section(t_data *data, void *win, int *y,
						const char *title);
void				setup_transform_sliders(t_data *data, t_object *obj);
void				setup_color_sliders(t_data *data, t_object *obj);
void				setup_material_sliders(t_data *data, t_object *obj);
void				setup_property_sliders(t_data *data, t_object *obj);

void				setup_ambient_sliders(t_data *data, t_object *obj,
						int slider_id);

void				setup_light_sliders(t_data *data);
void				destroy_material(void *o);
t_texture			*load_texture(char *path, mlx_context mlx);

#endif // NINIRT_BONUS_H
