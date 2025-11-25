NAME = game
CC = cc

SRCS_DIR = srcs
OBJS_DIR = objs

SRCS = $(SRCS_DIR)/main.c \
        $(SRCS_DIR)/player.c \
        $(SRCS_DIR)/raycast.c

OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)

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

$(NAME): $(OBJS_DIR) $(OBJS)
	@echo "$(BLUE)🔗 Linking $(NAME)...$(RESET)"
	$(CC) $(OBJS) -o $(NAME) $(MLX_LIB) $(LFLAGS)

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c
	@echo "$(YELLOW)🔨 Compiling $<...$(RESET)"
	$(CC) -c $< -o $@

clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	rm -rf $(OBJS_DIR)

fclean: clean
	@echo "$(RED)🗑️ Removing $(NAME)...$(RESET)"
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re