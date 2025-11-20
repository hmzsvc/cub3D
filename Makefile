NAME = game
CC = cc
SRC = srcs/main.c srcs/player.c
OBJ = $(SRC:.c=.o)

LFLAGS = -L./minilibx-linux -lmlx -lXext -lX11 -lm -lz
MLX_LIB = ./minilibx-linux/libmlx.a

# Colors
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[1;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)
	@echo "$(GREEN)✅ $(NAME) is ready! 🎮$(RESET)"

$(NAME): $(OBJ)
	@echo "$(BLUE)🔗 Linking $(NAME)...$(RESET)"
	$(CC) $(OBJ) -o $(NAME) $(MLX_LIB) $(LFLAGS)

%.o: %.c
	@echo "$(YELLOW)🔨 Compiling $<...$(RESET)"
	$(CC) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	rm -rf $(OBJ)

fclean: clean
	@echo "$(RED)🗑️ Removing $(NAME)...$(RESET)"
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re