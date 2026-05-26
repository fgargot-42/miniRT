/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_mapping.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fgargot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 18:21:30 by fgargot           #+#    #+#             */
/*   Updated: 2026/05/26 19:16:30 by fgargot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

t_vec2 get_uv(t_vec3 vec)
{
	double a;
	double b;
	t_vec2 result;

	a = atan2(vec.z, vec.x);
	b = fmax(-1.0, fmin(1.0, vec.y));
	b = acos(b);
	result.x = (a+M_PI) / (2 * M_PI);
	result.y = b / M_PI;
	return (result);
}


t_vec3 uv_to_color(t_texture *tex, t_vec2 uv, void *mlx)
{
    t_vec3 		col;
	mlx_color	pixel;
    int 		x;
    int 		y;

	x = (int)(uv.x * (tex->width - 1));
	y = (int)(uv.y * (tex->height - 1));
	pixel = mlx_get_image_pixel(mlx, tex->data, x, y);
	col.x = pixel.r;
	col.y = pixel.g;
	col.z = pixel.b;
    return (col);
}

t_vec3	triangle_uv_to_color(t_object *obj, t_vec3 hit, void *mlx)
{
	t_vec3		obj_vec[3];
	double		dot[2][3];
	double		det;
	t_vec2		uv;
	mlx_color	pixel;

	obj_vec[0] = vec3_sub(obj->props.b, obj->props.a);
	obj_vec[1] = vec3_sub(obj->props.c, obj->props.a);
	obj_vec[2] = vec3_sub(hit, obj->props.a);
	dot[0][0] = vec3_dot(obj_vec[0], obj_vec[0]);
	dot[0][1] = vec3_dot(obj_vec[0], obj_vec[1]);
	dot[0][2] = vec3_dot(obj_vec[0], obj_vec[2]);
	dot[1][1] = vec3_dot(obj_vec[1], obj_vec[1]);
	dot[1][2] = vec3_dot(obj_vec[1], obj_vec[2]);
	det = dot[0][0] * dot[1][1] - pow(dot[0][1], 2);
	if (fabs(det) < 1e-10)
		return (t_vec3){0, 0, 0};
	uv.x = (dot[1][1] * dot[0][2] - dot[0][1] * dot[1][2]) / det;
	uv.y = (dot[0][0] * dot[1][2] - dot[0][1] * dot[0][2]) / det;
	uv = vec2_add(vec2_add(obj->texture.tex_a,
		vec2_scale(vec2_sub(obj->texture.tex_b, obj->texture.tex_a), uv.x)),
		vec2_scale(vec2_sub(obj->texture.tex_c, obj->texture.tex_a), uv.y));
	uv.x = uv.x * obj->tex->width - 1;
	uv.y = (1 - uv.y) * obj->tex->height - 1;
	pixel = mlx_get_image_pixel(mlx, obj->tex->data, uv.x, uv.y);
	return ((t_vec3){pixel.r, pixel.g, pixel.b});
}

t_texture *load_texture(char *path, void *mlx)
{
    t_texture	*tex;

	tex = ft_calloc(1, sizeof(t_texture));
	if (!tex)
		return (NULL);
	tex->data = mlx_new_image_from_file(mlx, path, &tex->width, &tex->height);
    if (!tex->data)
    {
        printf("Failed to load image\n");
		free(tex);
		return (NULL);
    }
	printf("Image loaded: %s (%i x %i)\n", path, tex->width, tex->height);
    return (tex);
}
