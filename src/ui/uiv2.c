#include "miniRT.h"
#include "mlx.h"
#include <stdarg.h>
#include <stdio.h>

#define COL_BG        (mlx_color){.r = 30,  .g = 30,  .b = 46,  .a = 255}
#define COL_TITLEBAR  (mlx_color){.r = 45,  .g = 74,  .b = 122, .a = 255}
#define COL_BORDER    (mlx_color){.r = 74,  .g = 111, .b = 165, .a = 255}
#define COL_LABEL     (mlx_color){.r = 112, .g = 128, .b = 144, .a = 255}
#define COL_VALUE     (mlx_color){.r = 240, .g = 208, .b = 96,  .a = 255}
#define COL_ADDR      (mlx_color){.r = 100, .g = 180, .b = 232, .a = 255}
#define COL_TYPE      (mlx_color){.r = 120, .g = 232, .b = 120, .a = 255}
#define COL_SECTION   (mlx_color){.r = 90,  .g = 138, .b = 90,  .a = 255}
#define COL_SEP       (mlx_color){.r = 42,  .g = 74,  .b = 42,  .a = 255}
#define COL_FOOTER    (mlx_color){.r = 64,  .g = 80,  .b = 96,  .a = 255}
#define COL_WHITE     (mlx_color){.rgba = MLX_WHITE}
#define COL_TRACK_BG  (mlx_color){.r = 18,  .g = 18,  .b = 30,  .a = 255}

#define PANEL_X       0
#define PANEL_Y       0
#define PANEL_W       430
#define PANEL_PAD     10
#define LINE_H        20
#define TITLE_H       22
#define COL_VALUES_X  (PANEL_X + PANEL_W / 2)

static void	fill_rect(t_data *data, void *win,
					int x, int y, int w, int h, mlx_color col)
{
	int	px;
	int	py;

	py = y;
	while (py < y + h)
	{
		px = x;
		while (px < x + w)
			mlx_pixel_put(data->mlx, win, px++, py, col);
		py++;
	}
}

static void	draw_border(t_data *data, void *win,
					int x, int y, int w, int h, mlx_color col)
{
	int	i;

	i = x;
	while (i < x + w)
	{
		mlx_pixel_put(data->mlx, win, i, y, col);
		mlx_pixel_put(data->mlx, win, i, y + h - 1, col);
		i++;
	}
	i = y;
	while (i < y + h)
	{
		mlx_pixel_put(data->mlx, win, x, i, col);
		mlx_pixel_put(data->mlx, win, x + w - 1, i, col);
		i++;
	}
}

static void	draw_hline(t_data *data, void *win, int y)
{
	int	x;

	x = PANEL_X + PANEL_PAD;
	while (x < PANEL_X + PANEL_W - PANEL_PAD)
		mlx_pixel_put(data->mlx, win, x++, y, COL_SEP);
}

static void	put_row(t_data *data, void *win, int *y,
					const char *label, mlx_color val_col, const char *fmt, ...)
{
	char	buf[128];
	va_list	args;

	va_start(args, fmt);
	vsnprintf(buf, sizeof(buf), fmt, args);
	va_end(args);
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, win, PANEL_X + PANEL_PAD, *y, COL_LABEL, (char *)label);
	mlx_string_put(data->mlx, win, COL_VALUES_X, *y, val_col, buf);
	*y += LINE_H;
}

static void	put_section(t_data *data, void *win, int *y, const char *title)
{
	*y += 12;
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, win, PANEL_X + PANEL_PAD, *y, COL_SECTION, (char *)title);
	*y += LINE_H - 6;
	draw_hline(data, win, *y);
	*y += 16;
}

void	init_editor(t_data *data)
{
	mlx_window_create_info	info;

	if (data->editor)
		return ;
	info = (mlx_window_create_info){
		.title = "inspector",
		.width = EDITOR_W,
		.height = EDITOR_H,
	};
	data->editor = mlx_new_window(data->mlx, &info);
	if (!data->editor)
		exit(1);
	mlx_set_window_size(data->mlx, data->editor, EDITOR_W, EDITOR_H);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEDOWN,
		editor_mouse_down, data);
	mlx_on_event(data->mlx, data->editor, MLX_MOUSEUP,
		editor_mouse_up, data);
}
static const char	*obj_type_name(int type)
{
	static const char	*names[] = {
		[OBJ_SPHERE]   = "SPHERE",
		[OBJ_PLANE]    = "PLANE",
		[OBJ_CYLINDER] = "CYLINDER",
		[OBJ_CONE]     = "CONE",
	};
	if (type < 0 || type >= (int)(sizeof(names) / sizeof(*names)))
		return ("UNKNOWN");
	return (names[type]);
}




void	setup_sliders(t_data *data)
{
	t_object	*obj;
	t_vec3		*pos;
	t_vec3		*col;
	double		*spec;
	double		*shin;
	double		*radius;
	double		*height;
	int			i;

	static const char *position_labels[3] = {"pos.x", "pos.y", "pos.z"};
	static const char *color_labels[3] = {"col.r", "col.g", "col.b"};
	static const char *material_labels[2] = {"specular", "shininess"};

	static const char *properties_labels[2] = {"radius", "height"};


	static const mlx_color position_colors[3] = {
		{.r = 100, .g = 200, .b = 255, .a = 255},
		{.r = 100, .g = 255, .b = 130, .a = 255},
		{.r = 255, .g = 150, .b = 100, .a = 255}
	};

	static const mlx_color color_colors[3] = {
		{.r = 255, .g = 80,  .b = 80,  .a = 255},
		{.r = 80,  .g = 220, .b = 80,  .a = 255},
		{.r = 80,  .g = 140, .b = 255, .a = 255}
	};

	static const mlx_color material_colors[2] = {
		{.r = 180, .g = 180, .b = 255, .a = 255},
		{.r = 200, .g = 200, .b = 200, .a = 255}
	};
	data->nb_sliders = 0;
	obj = data->scene->selected;
	if (!obj)
		return ;

	pos = NULL;
	col = NULL;
	spec = NULL;
	shin = NULL;
	radius = NULL;
	height = NULL;

	if (obj->type == OBJ_SPHERE)
	{
		pos = &((t_sphere *)obj->object)->center;
		col = &((t_sphere *)obj->object)->color;
		spec = &((t_sphere *)obj->object)->specular;
		shin = &((t_sphere *)obj->object)->shininess;
		radius = &((t_sphere *)obj->object)->radius;
	}
	else if (obj->type == OBJ_PLANE)
	{
		pos = &((t_plane *)obj->object)->point;
		col = &((t_plane *)obj->object)->color;
		spec = &((t_plane *)obj->object)->specular;
		shin = &((t_plane *)obj->object)->shininess;
	}
	else if (obj->type == OBJ_CYLINDER)
	{
		pos = &((t_cylinder *)obj->object)->center;
		col = &((t_cylinder *)obj->object)->color;
		spec = &((t_cylinder *)obj->object)->specular;
		shin = &((t_cylinder *)obj->object)->shininess;
		radius = &((t_cylinder *)obj->object)->radius;
		height = &((t_cylinder *)obj->object)->height;
	}
	else if (obj->type == OBJ_CONE)
	{
		pos = &((t_cone *)obj->object)->center;
		col = &((t_cone *)obj->object)->color;
		spec = &((t_cone *)obj->object)->specular;
		shin = &((t_cone *)obj->object)->shininess;
		radius = &((t_cone *)obj->object)->angle;
		height = &((t_cone *)obj->object)->height;
	}
	if (!pos || !col)
		return ;

	// POSITION (0–2)
	i = 0;
	while (i < 3)
	{
		data->sliders[i] = (t_slider){
			.value = ((double *)pos) + i,
			.min   = -SLD_POS_RANGE,
			.max   =  SLD_POS_RANGE,
			.label = position_labels[i],
			.col   = position_colors[i],
		};
		i++;
	}

	// COLOR (3–5)
	i = 0;
	while (i < 3)
	{
		data->sliders[3 + i] = (t_slider){
			.value = ((double *)col) + i,
			.min   = 0.0,
			.max   = 255.0,
			.label = color_labels[i],
			.col   = color_colors[i],
		};
		i++;
	}

	// MATERIAL (6–7)
	data->sliders[6] = (t_slider){
		.value = spec,
		.min   = 0.0,
		.max   = 1.0,
		.label = material_labels[0],
		.col   = material_colors[0],
	};

	data->sliders[7] = (t_slider){
		.value = shin,
		.min   = 1.0,
		.max   = 1000.0,
		.label = material_labels[1],
		.col   = material_colors[1],
	};
	
	data->sliders[8] = (t_slider){
		.value = radius,
		.min   = 0.0,
		.max   = 100.0,
		.label = properties_labels[0],
		.col   = material_colors[1],
	};

	data->sliders[9] = (t_slider){
		.value = height,
		.min   = 0.0,
		.max   = 100.0,
		.label = properties_labels[1],
		.col   = material_colors[1],
	};


	data->nb_sliders = 10;
}



/*
    PANEL_PAD           SLD_X          SLD_X+SLD_W    SLD_X+SLD_W+50
    label               [====filled====|----empty----]    value
                                       ^ curseur
*/
static void	draw_slider(t_data *data, t_slider *s)
{
	double		t;
	//int			thumb_x;
	int			filled_w;
	char		buf[32];
	mlx_color	dim;
	if (s->value == NULL)
		return ;

	//clear slider
	fill_rect(data, data->editor,
    0,
    s->y - 6,
    EDITOR_W,
    SLD_H + 12,
    COL_BG);

	// fond fonce
	dim = (mlx_color){
		.r = s->col.r / 5,
		.g = s->col.g / 5,
		.b = s->col.b / 5,
		.a = 255
	};
	t = (*s->value - s->min) / (s->max - s->min);
	if (t < 0.0)
		t = 0.0;
	if (t > 1.0)
		t = 1.0;
	filled_w = (int)(t * SLD_W);
//	thumb_x  = SLD_X + filled_w;
	// bg
	fill_rect(data, data->editor, SLD_X, s->y - 1, SLD_W, SLD_H + 2, dim);
	if (filled_w > 0)
		fill_rect(data, data->editor, SLD_X, s->y - 1, filled_w, SLD_H + 2, s->col);
	// TODO ptit truc blanc
//	fill_rect(data, data->editor,
//		thumb_x - 3, s->y - 4, 6, SLD_H + 8, COL_WHITE);
	// label
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, data->editor,
		PANEL_PAD, s->y, COL_LABEL, (char *)s->label);
	//value
	snprintf(buf, sizeof(buf), "%.2f", *s->value);
	printf("%s : %.2f\n", (char*)s->label, *s->value);
	mlx_string_put(data->mlx, data->editor,
		SLD_X + SLD_W + 8, s->y, COL_VALUE, buf);
}

void	draw_editor(t_data *data)
{
	int	y;
	int	i;

	if (!data->editor || data->nb_sliders == 0)
		return ;

	fill_rect(data, data->editor,
		0, SLD_BASE_Y - 44,
		EDITOR_W, EDITOR_H - (SLD_BASE_Y - 44),
		COL_BG);

	y = SLD_BASE_Y - 44;

	put_section(data, data->editor, &y, "TRANSFORM -");
	i = 0;
	while (i < 3)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}

	y += 12;
	put_section(data, data->editor, &y, "COLOR -----");
	i = 3;
	while (i < 6)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}

	y += 12;
	put_section(data, data->editor, &y, "MATERIAL -----");
	i = 6;
	while (i < 8)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}


	y += 12;
	put_section(data, data->editor, &y, "PROPERTIES -----");
	i = 8;
	while (i < 10)
	{
		data->sliders[i].y = y;
		draw_slider(data, &data->sliders[i]);
		y += SLD_SPACING;
		i++;
	}

	draw_hline(data, data->editor, y + 4);
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(data->mlx, data->editor,
		PANEL_PAD, y + 4,
		COL_FOOTER,
		"fgargot && mabarrer | miniRT");
}

void	print_hit_info(t_data *data, t_hit_record hit,
					double mouse_x, double mouse_y)
{
	int	y;
	int	panel_h;

	init_editor(data);

	mlx_clear_window(data->mlx, data->editor, COL_WHITE);

	if (!hit.object)
		return ;
	mlx_set_window_size(data->mlx, data->editor, EDITOR_W, EDITOR_H);
	panel_h = TITLE_H + LINE_H * 10 + 60;
	mlx_set_font(data->mlx, "resources/font.ttf");
	

	// bg + title + border
	fill_rect(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, panel_h, COL_BG);
	fill_rect(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, TITLE_H, COL_TITLEBAR);
	draw_border(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, panel_h, COL_BORDER);
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 14.0f);
	mlx_string_put(data->mlx, data->editor,
		PANEL_X + PANEL_PAD, PANEL_Y + 16, COL_WHITE, "miniRT INSPECTOR |      ☺");


	y = PANEL_Y + TITLE_H + 8;


	// object section
	put_section(data, data->editor, &y, "OBJECT ----");
	put_row(data, data->editor, &y, "addr",   COL_ADDR,  "%p",  hit.object);
	put_row(data, data->editor, &y, "type",   COL_TYPE,  "%s",  obj_type_name(hit.object->type));

	// hit section
	put_section(data, data->editor, &y, "HIT ------");
	put_row(data, data->editor, &y, "point",  COL_VALUE, "%.3f  %.3f  %.3f",
		hit.point.x, hit.point.y, hit.point.z);
	put_row(data, data->editor, &y, "mouse",  COL_VALUE, "u=%.1f  v=%.1f",
		mouse_x, mouse_y);

	// surface
	put_section(data, data->editor, &y, "SURFACE --");
	put_row(data, data->editor, &y, "normal", COL_VALUE, "%.3f  %.3f  %.3f",
		hit.normal.x, hit.normal.y, hit.normal.z);
	put_row(data, data->editor, &y, "color",  COL_VALUE, "%.3f  %.3f  %.3f",
		hit.color.x, hit.color.y, hit.color.z);

	setup_sliders(data);
	draw_editor(data);
}
