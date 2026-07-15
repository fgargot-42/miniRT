# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgargot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/05/29 18:52:27 by fgargot           #+#    #+#              #
#    Updated: 2026/07/15 21:42:11 by fgargot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -march=native -flto -ffast-math -funroll-loops -MMD -MP
CFLAGS_DEBUG = $(CFLAGS:-O3=-g)
CLINK = -lSDL2 -lm -lpthread
SRCDIR = src
OBJDIR = obj
INCDIR=includes
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
MLX_DIR = lib/MacroLibX
MLX = $(MLX_DIR)/libmlx.so
INCLUDE = -Iincludes -I$(LIBFT_DIR)/includes -I$(MLX_DIR)/includes
SRC =	main.c \
		bvh/bvh.c \
		bvh/bvh_aabb.c \
		bvh/bvh_aabb2.c \
		bvh/bvh_init.c \
		bvh/bvh_split.c \
		bvh/bvh_split2.c \
		bvh/bvh_size.c \
		bvh/bvh_sort.c \
		bvh/bvh_utils.c \
		bvh/sah_split.c \
		bvh/sah_utils.c \
		debug/print_debug.c \
		debug/print_debug2.c \
		input/hooks.c \
		input/mouse_hooks.c \
		input/mouse_hooks_editor.c \
		camera/camera.c \
		camera/move_camera.c \
		parser/rt_parser/parser.c \
		parser/rt_parser/parser_utils.c \
		parser/rt_parser/parse_number.c \
		parser/rt_parser/parse_ambient.c \
		parser/rt_parser/parse_camera.c \
		parser/rt_parser/parse_sky.c \
		parser/rt_parser/parse_light.c \
		parser/rt_parser/parse_sphere.c \
		parser/rt_parser/parse_plane.c \
		parser/rt_parser/parse_cylinder.c \
		parser/rt_parser/parse_cone.c \
		parser/rt_parser/parse_hyperboloid.c \
		parser/rt_parser/parse_paraboloid.c \
		parser/rt_parser/parse_triangle.c \
		parser/rt_parser/parse_texture.c \
		parser/obj_parser/obj_parser.c \
		parser/obj_parser/mat_parser.c \
		parser/obj_parser/parse_material.c \
		parser/obj_parser/parse_material2.c \
		parser/obj_parser/parse_obj_face.c \
		parser/obj_parser/parse_obj_model.c \
		parser/obj_parser/parser_texture.c \
		parser/obj_parser/obj_parser_utils.c \
		renderer/drawer.c \
		renderer/drawer_pixel.c \
		renderer/drawer_utils.c \
		renderer/debug.c \
		renderer/print_debug.c \
		scene/scene.c \
		hit/hit_sphere.c \
		hit/hit_plane.c \
		hit/hit_cylinder.c \
		hit/hit_cone.c \
		hit/hit_hyperboloid.c \
		hit/hit_paraboloid.c \
		hit/hit_triangle.c \
		hit/hit_bvh.c \
		hit/hit_bvh_utils.c \
		hit/hit_utils.c \
		hit/hit.c \
		hit/ray.c \
		lighting/lighting.c \
		lighting/lighting_utils.c \
		veclib/veclib2_math.c \
		veclib/veclib2_math2.c \
		veclib/veclib3_math.c \
		veclib/veclib3_math2.c \
		veclib/veclib3_math3.c \
		matlib/matrix.c \
		matlib/matrix2.c \
		matlib/matrix3.c \
		matlib/matrix_utils.c \
		utils/polynom.c \
		utils/file_utils.c \
		utils/array_utils.c \
		utils/str_utils.c \
		utils/color.c \
		ui/ui.c \
		ui/helpers.c \
		ui/setup.c \
		ui/setup2.c \
		uv_mapping.c
OBJ = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))
OBJ_DEBUG = $(addprefix $(OBJDIR)_debug/,$(SRC:.c=.o))

all: $(NAME)

$(MLX):
	make -j16 -C $(MLX_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(MLX) $(OBJ) $(LIBFT) 
	$(CC) $(CFLAGS) $(CLINK) $(INCLUDE) $^ -o $@ 

debug: $(MLX) $(OBJ_DEBUG) $(LIBFT)
	$(CC) $(CFLAGS_DEBUG) $(CLINK) $(INCLUDE) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

$(OBJDIR)_debug/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS_DEBUG) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(OBJDIR)_debug
	make -C $(MLX_DIR) clean
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(MLX_DIR) fclean
	make -C $(LIBFT_DIR) fclean

re: fclean all

-include $(OBJ:.o=.d)
-include $(OBJ_DEBUG:.o=.d)

.PHONY: all clean fclean re
