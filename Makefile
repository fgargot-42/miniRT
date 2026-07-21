# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgargot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/29 18:52:27 by fgargot           #+#    #+#              #
#    Updated: 2026/07/21 23:42:34 by mabarrer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CFLAGS = -Wall -Wextra -Werror -O3 -march=native -flto -ffast-math -funroll-loops -MMD -MP

ifeq ($(DEBUG), true)
	CC = CCC_OVERRIDE_OPTIONS="\# O0 +-g" cc
else
	CC = cc
endif

CLINK = -lSDL2 -lm
CLINK_BONUS = -lSDL2 -lm -lpthread
SRCDIR = src
OBJDIR = obj
INCDIR = includes
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = lib/MacroLibX
MLX = $(MLX_DIR)/libmlx.so
INCLUDE = -Iincludes -I$(LIBFT_DIR)/includes -I$(MLX_DIR)/includes
SRC =	bvh/bvh.c \
		bvh/bvh_aabb_object.c \
		bvh/bvh_init.c \
		bvh/bvh_split2.c \
		bvh/bvh_size.c \
		bvh/sah_split.c \
		bvh/sah_utils.c \
		camera/camera.c \
		hit/hit_bvh.c \
		hit/hit_bvh_utils.c \
		hit/ray.c \
		input/hooks.c \
		input/mouse_hooks.c \
		lighting/lighting_utils.c \
		parser/rt_parser/parser_utils.c \
		parser/rt_parser/parse_number.c \
		parser/rt_parser/parse_ambient.c \
		parser/rt_parser/parse_camera.c \
		renderer/drawer_utils.c \
		veclib/veclib2_math.c \
		veclib/veclib2_math2.c \
		veclib/veclib3_math.c \
		veclib/veclib3_math2.c \
		veclib/veclib3_math3.c \
		matlib/matrix2.c \
		matlib/matrix3.c \
		matlib/matrix_utils.c \
		ui/helpers.c \
		ui/ui_draw.c \
		ui/ui_init.c \
		utils/polynom.c \
		utils/file_utils.c \
		utils/array_utils.c \
		utils/str_utils.c \
		utils/time_utils.c \
		utils/color.c \

SRC_MANDA = $(SRC) \
		main.c \
		bvh/bvh_aabb.c \
		bvh/bvh_split.c \
		bvh/bvh_utils.c \
		camera/move_camera.c \
		display/init_display.c \
		hit/hit.c \
		hit/hit_sphere.c \
		hit/hit_plane.c \
		hit/hit_cylinder.c \
		hit/hit_utils.c \
		lighting/lighting.c \
		input/mouse_hooks_editor.c \
		parser/rt_parser/parser.c \
		parser/rt_parser/parse_light.c \
		parser/rt_parser/parse_sphere.c \
		parser/rt_parser/parse_plane.c \
		parser/rt_parser/parse_cylinder.c \
		renderer/drawer.c \
		renderer/drawer_pixel.c \
		scene/scene.c \
		scene/scene_utils.c \
		ui/setup.c \
		ui/ui.c \
		ui/ui_helpers.c \
		ui/ui_slider_header.c \
		ui/setup2.c 
SRC_BONUS = $(SRC) \
		main_bonus.c \
		bvh/bvh_aabb_bonus.c \
		bvh/bvh_aabb_object_bonus.c \
		bvh/bvh_split_bonus.c \
		bvh/bvh_utils_bonus.c \
		camera/move_camera_bonus.c \
		display/init_display_bonus.c \
		input/mouse_hooks_editor_bonus.c \
		hit/hit_bonus.c \
		hit/hit_sphere_bonus.c \
		hit/hit_plane_bonus.c \
		hit/hit_cylinder_bonus.c \
		hit/hit_cone_bonus.c \
		hit/hit_hyperboloid_bonus.c \
		hit/hit_paraboloid_bonus.c \
		hit/hit_triangle_bonus.c \
		hit/hit_utils_bonus.c \
		lighting/lighting_bonus.c \
		parser/obj_parser/obj_parser_bonus.c \
		parser/obj_parser/mat_parser_bonus.c \
		parser/obj_parser/parse_material_bonus.c \
		parser/obj_parser/parse_material2_bonus.c \
		parser/obj_parser/parse_obj_face_bonus.c \
		parser/obj_parser/parse_obj_model_bonus.c \
		parser/obj_parser/parser_texture_bonus.c \
		parser/obj_parser/obj_parser_utils_bonus.c \
		parser/rt_parser/parser_bonus.c \
		parser/rt_parser/parse_sky_bonus.c \
		parser/rt_parser/parse_light_bonus.c \
		parser/rt_parser/parse_sphere_bonus.c \
		parser/rt_parser/parse_plane_bonus.c \
		parser/rt_parser/parse_cylinder_bonus.c \
		parser/rt_parser/parse_cone_bonus.c \
		parser/rt_parser/parse_hyperboloid_bonus.c \
		parser/rt_parser/parse_paraboloid_bonus.c \
		parser/rt_parser/parse_triangle_bonus.c \
		parser/rt_parser/parse_texture_bonus.c \
		renderer/drawer_bonus.c \
		renderer/drawer_pixel_bonus.c \
		scene/scene_bonus.c \
		scene/scene_utils_bonus.c \
		ui/ui_bonus.c \
		ui/setup_bonus.c \
		ui/setup2_bonus.c \
		ui/ui_helpers_bonus.c \
		ui/ui_slider_header_bonus.c \
		uv_mapping_bonus.c

OBJ_MANDA = $(addprefix $(OBJDIR)/,$(SRC_MANDA:.c=.o))
OBJ_BONUS = $(addprefix $(OBJDIR)/,$(SRC_BONUS:.c=.o))

all: $(NAME)

manda: $(OBJ_MANDA) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(CLINK_BONUS) $(INCLUDE) $^ -o $(NAME)

bonus: $(OBJ_BONUS) $(LIBFT) $(MLX)
	$(CC) $(CFLAGS) $(CLINK_BONUS) $(INCLUDE) $^ -o $(NAME)

$(MLX):
	make -j16 -C $(MLX_DIR) DEBUG=false

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(OBJ_MANDA) $(LIBFT) $(MLX) 
	$(CC) $(CFLAGS) $(CLINK) $(INCLUDE) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(OBJDIR)_debug
	make -C $(MLX_DIR) clean
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(MLX_DIR) fclean
	make -C $(LIBFT_DIR) fclean

re: fclean all

-include $(OBJ_MANDA:.o=.d)
-include $(OBJ_BONUS:.o=.d)

.PHONY: all manda bonus clean fclean re
