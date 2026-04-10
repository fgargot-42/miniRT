NAME = miniRT
CC = cc
CFLAGS = -Wall -Wextra -Werror -O3 -march=native -funroll-loops -flto -ffast-math -fstrict-aliasing
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
		ray.c \
		camera.c \
		hooks.c \
		hit/hit_sphere.c \
		hit/hit_plane.c \
		hit/hit_cylinder.c \
		hit/hit_cone.c \
		hit/hit.c \
		lighting.c \
		veclib/veclib_math.c \
		veclib/veclib_math2.c \
		veclib/matrix.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(MLX):
	make -j16 -C $(MLX_DIR)

$(LIBFT):
	make -C $(LIBFT_DIR)

$(NAME): $(MLX) $(addprefix $(OBJDIR)/, $(OBJ)) $(LIBFT) 
	$(CC) $(CFLAGS) $(CLINK) $(INCLUDE) $^ -o $@ 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	make -C $(MLX_DIR) clean
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(MLX_DIR) fclean
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
