/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/21 18:43:41 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/29 23:53:42 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "types.h"
# include "veclib.h"
# include "mlx.h"
# include "math.h"
# include "SDL2/SDL_scancode.h"
# include "ui.h"
# include "libft.h"
# include "material.h"

# define T_MIN 0.001
# define T_MAX 100
# define WIDTH 1200
# define HEIGHT 720
# define CAMERA_SENS 0.35
# define MOVE_STEP 0.5
# define NB_THREADS 16
# define BVH_DEPTH 10
# define DEBUG 1
# define BVH_VIEW 1
# define MLX_WHITE 0xFFFFFFFF

// BVH

typedef void		(*t_obj_aabb_fn)(t_object *, t_vec3 *, t_vec3 *);

void				get_sphere_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				get_cylinder_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				get_cone_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				get_hyperboloid_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				get_paraboloid_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				get_triangle_aabb(t_object *obj, t_vec3 *aabb_min,
						t_vec3 *aabb_max);

void				bvh_grow_to_include(t_bvh *bvh, t_object *object);
void				bvh_grow_all_to_include(t_bvh *root, t_object *object);
int					is_bvh_object(void *e);
void				get_box_aabb(t_list *elements, t_vec3 *aabb_min,
						t_vec3 *aabb_max);
void				sort_bvh_objects_asc(t_object **array, int min, int max,
						char axis);
t_bvh				*build_bvh_tree(t_scene *scene);
void				bvh_destroy_tree(t_bvh **bvh);
void				rebuild_bvh_tree(t_bvh **bvh, t_scene *scene);
int					bvh_split(t_bvh *bvh, t_vec3 left_bound, int depth);
t_vec3				get_left_bounds(t_bvh *bvh);
t_vec3				get_object_center(t_object *obj);

// OBJECTS

t_object			*create_object(void *object, t_obj_type type);

// DEBUG

void				print_sky(t_object *sky);
void				print_object(void *o);
void				print_bvh_tree(t_bvh *bvh, int depth);

// SCENE
void				init_scene(char *file, t_data *data);
void				free_scene(t_scene *scene);
void				free_object(void *object);

// DRAWER

void				draw(t_data *data);
void				draw_single(t_data *data);
void				add_debug(t_data *data);
void				print_hit_info(t_data *data, t_hit_record hc,
						double mouse_x, double mouse_y);
void				print_hit_info_debug(t_hit_record hc, t_scene *scene,
						t_ray *ray, t_vec2 mouse_pos);
mlx_color			vec3_to_color(t_vec3 v);

//src/hooks.c
void				attach_hooks(t_data *data);

void				mouse_down_hook(int mouse_event, void *param);
void				mouse_up_hook(int mouse_event, void *param);
void				mouse_wheel_hook(int mouse_event, void *param);

//src/camera.c
t_ray				camera_ray(t_object *cam, int x, int y);
void				mouse_loop(void *param);

//src/hit.c
t_vec3				face_normal(t_ray *ray, t_vec3 inverted);
int					hit_list(t_list *obj, t_ray *ray, double *closest,
						t_hit_record *rec);
int					hit_scene(t_scene *scene, t_ray *ray, double t_max,
						t_hit_record *rec, int display_bvh_depth);
int					hit_sphere(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_plane(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_cylinder(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_cone(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_hyperboloid(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_paraboloid(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_triangle(t_object *obj, t_ray *ray, double t_max,
						t_hit_record *rec);
int					hit_bvh_box(t_bvh *bvh, t_ray *ray, double *dist, t_vec3 *point);

//src/ray.c
t_vec3				ray_at(t_ray ray, double t);
t_ray				get_object_relative_ray(t_ray ray, t_object *obj);

//lighting.c
t_vec3				shade(t_hit_record *rec, t_scene *scene, t_ray *ray);

// UTILS

int					get_polynom2_roots(double *roots, double a, double b,
						double c);
int					open_file_read(char *file, char *extension);
char				*get_directory_path(char *filepath);
void				free_str_array(char **array);
size_t				get_str_array_length(char **array);

t_vec3				srgb_to_linear(t_vec3 srgb);
t_vec3				linear_to_srgb(t_vec3 linear_rgb);

// ui
void				init_editor(t_data *data);

void				init_editor(t_data *data);
void				setup_sliders(t_data *data);
void				draw_editor(t_data *data);
void				editor_mouse_down(int event, void *param);
void				editor_mouse_up(int event, void *param);
void				editor_loop(void *param);

void				fill_rect(t_data *data, t_vec2 pos, t_vec2 size,
						mlx_color col);

void				draw_hline(t_data *data, void *win, int y);

void				put_row(t_data *data, void *win, int *y,
						const char *label, mlx_color val_col, const char *fmt, ...);
void				put_section(t_data *data, void *win, int *y,
						const char *title);
void				setup_transform_sliders(t_data *data, t_object *obj);
void				setup_color_sliders(t_data *data, t_object *obj);
void				setup_material_sliders(t_data *data, t_object *obj);
void				setup_property_sliders(t_data *data, t_object *obj);

void				setup_ambient_sliders(t_data *data, t_object *obj);

t_texture			*load_texture(char *path, void *mlx);
t_vec3				uv_to_color(t_texture *tex, t_vec2 uv, void *mlx);
t_vec3				triangle_uv_to_color(t_object *obj, t_vec3 hit, void *mlx);
t_vec2				get_uv(t_vec3 vec);

#endif
