#include "miniRT.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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


t_vec3 uv_to_color(t_texture *tex, t_vec2 uv)
{
    int x = (int)(uv.x * (tex->width - 1));
    int y = (int)(uv.y * (tex->height - 1));

    int index = (y * tex->width + x) * tex->channels;

    t_vec3 col;

    col.x = tex->data[index + 0] / 255.0;
    col.y = tex->data[index + 1] / 255.0;
    col.z = tex->data[index + 2] / 255.0;

    return col;
}

t_texture load_texture(const char *path)
{
    t_texture tex;

    tex.data = stbi_load(path, &tex.width, &tex.height, &tex.channels, 0);
    if (!tex.data)
    {
        printf("Failed to load image\n");
        exit(1);
    }
    return tex;
}
