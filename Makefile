CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes -I$(MLX_DIR)/includes
LIBS = -lSDL2 -lm

NAME = miniRT
SRCDIR = src
MLX_DIR = lib/MacroLibX

SOURCES = main.c ray.c camera.c hooks.c hit.c
VECLIB_SOURCES = veclib/veclib.c
SRCS = $(addprefix $(SRCDIR)/, $(SOURCES) $(VECLIB_SOURCES))
OBJS = $(SRCS:.c=.o)
MLX = $(MLX_DIR)/libmlx.so

all: $(NAME)

$(MLX):
	make -j16 -C $(MLX_DIR)

$(NAME): $(MLX) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(MLX) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)
	make -C $(MLX_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(MLX_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
