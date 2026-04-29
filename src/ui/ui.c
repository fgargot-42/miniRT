#include "miniRT.h"
#include <stdarg.h>

// color palette
#define COL_BG       (mlx_color){.r = 30,  .g = 30,  .b = 46,  .a = 255}
#define COL_TITLEBAR (mlx_color){.r = 45,  .g = 74,  .b = 122, .a = 255}
#define COL_BORDER   (mlx_color){.r = 74,  .g = 111, .b = 165, .a = 255}
#define COL_LABEL    (mlx_color){.r = 112, .g = 128, .b = 144, .a = 255}
#define COL_VALUE    (mlx_color){.r = 240, .g = 208, .b = 96,  .a = 255}
#define COL_ADDR     (mlx_color){.r = 100, .g = 180, .b = 232, .a = 255}
#define COL_TYPE     (mlx_color){.r = 120, .g = 232, .b = 120, .a = 255}
#define COL_SECTION  (mlx_color){.r = 90,  .g = 138, .b = 90,  .a = 255}
#define COL_SEP      (mlx_color){.r = 42,  .g = 74,  .b = 42,  .a = 255}
#define COL_FOOTER   (mlx_color){.r = 64,  .g = 80,  .b = 96,  .a = 255}
#define COL_WHITE    (mlx_color){.rgba = MLX_WHITE}

// layout
#define PANEL_X      0
#define PANEL_Y      0
#define PANEL_W      430
#define PANEL_PAD    10
#define LINE_H       20
#define TITLE_H		 22
#define COL_VALUES_X (PANEL_X + PANEL_W / 2)

void	init_editor(t_data *data)
{
	mlx_window_create_info	info;

	if (data->editor)
		return ;
	info = (mlx_window_create_info){
		.title = "inspector",
		.width = 430,
		.height = 300,
	};
	data->editor = mlx_new_window(data->mlx, &info);
	if (!data->editor)
		exit(1);
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

// bordure 1px
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

// ligne hsep
static void	draw_hline(t_data *data, void *win, int y)
{
	int	x;

	x = PANEL_X + PANEL_PAD;
	while (x < PANEL_X + PANEL_W - PANEL_PAD)
		mlx_pixel_put(data->mlx, win, x++, y, COL_SEP);
}

// label  value
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

// titlbar
static void	put_section(t_data *data, void *win, int *y, const char *title)
{
	*y += 12;
	mlx_set_font_scale(data->mlx, "resources/font.ttf", 12.0f);
	mlx_string_put(data->mlx, win, PANEL_X + PANEL_PAD, *y, COL_SECTION, (char *)title);
	*y += LINE_H - 6;
	draw_hline(data, win, *y);
	*y += 16;
}

void	print_hit_info(t_data *data, t_hit_record hit, double mouse_x, double mouse_y)
{
	int	y;
	int	panel_h;
	init_editor(data);
	if (!hit.object)
		return ;
	// fond +cadre
	panel_h = TITLE_H + LINE_H * 10 + 60;
	mlx_set_font(data->mlx, "resources/font.ttf");

	fill_rect(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, panel_h, COL_BG);
	fill_rect(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, TITLE_H, COL_TITLEBAR);
	draw_border(data, data->editor, PANEL_X, PANEL_Y, PANEL_W, panel_h, COL_BORDER);

	mlx_set_font_scale(data->mlx, "resources/font.ttf", 14.0f);


	mlx_string_put(data->mlx, data->editor,
		PANEL_X + PANEL_PAD, PANEL_Y + 16, COL_WHITE, "INSPECTOR");
	// content
	y = PANEL_Y + TITLE_H + 8;
	put_section(data, data->editor, &y, "OBJECT ----");
	put_row(data, data->editor, &y, "addr",   COL_ADDR,  "%p",  hit.object);
	put_row(data, data->editor, &y, "type",   COL_TYPE,  "%s",  obj_type_name(hit.object->type));
	put_section(data, data->editor, &y, "HIT ------");
	put_row(data, data->editor, &y, "point",  COL_VALUE, "%.3f  %.3f  %.3f",
		hit.point.x, hit.point.y, hit.point.z);
	put_row(data, data->editor, &y, "mouse",  COL_VALUE, "u=%.1f  v=%.1f",
		mouse_x, mouse_y);
	put_section(data, data->editor, &y, "SURFACE --");
	put_row(data, data->editor, &y, "normal", COL_VALUE, "%.3f  %.3f  %.3f",
		hit.normal.x, hit.normal.y, hit.normal.z);
	put_row(data, data->editor, &y, "color",  COL_VALUE, "%.3f  %.3f  %.3f",
		hit.color.x, hit.color.y, hit.color.z);
	// credits
	draw_hline(data, data->editor, y + 4);

	mlx_set_font_scale(data->mlx, "resources/font.ttf", 8.0f);
	mlx_string_put(data->mlx, data->editor,
		PANEL_X + PANEL_PAD, y + 18, COL_FOOTER, "miniRT | fgargot && mabarrer");
}
