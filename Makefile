NAME = cub3D
CC = cc

SRCS_DIR = srcs
MAP_DIR = srcs/map
MAP_UTIL_DIR = srcs/map_utils_dir
OBJS_DIR = objs

# Libft
LIBFT_DIR = lib/libft
LIBFT = $(LIBFT_DIR)/libft.a

# Get Next Line
GNL_DIR = lib/get_next_line
GNL_SRCS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
GNL_OBJS = $(GNL_SRCS:$(GNL_DIR)/%.c=$(OBJS_DIR)/gnl/%.o)


SRCS = $(SRCS_DIR)/main.c \
        $(SRCS_DIR)/player.c \
        $(SRCS_DIR)/raycast.c \
		$(SRCS_DIR)/map/map_read.c \
		$(SRCS_DIR)/minimap.c \

SRCS_MAP = $(MAP_DIR)/load_texture.c \

SRCS_MAP_UTIL = $(MAP_UTIL_DIR)/map_utils.c \
					$(MAP_UTIL_DIR)/map_control.c \

OBJS_MAIN = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
OBJS_MAP = $(SRCS_MAP:$(MAP_DIR)/%.c=$(OBJS_DIR)/map/%.o)
OBJS_MAP_UTIL = $(SRCS_MAP_UTIL:$(MAP_UTIL_DIR)/%.c=$(OBJS_DIR)/map_utils/%.o)

OBJS = $(OBJS_MAIN) $(OBJS_MAP) $(OBJS_MAP_UTIL)

LFLAGS = -L./minilibx-linux -lmlx -lXext -lX11 -lm -lz
MLX_LIB = ./minilibx-linux/libmlx.a

# Colors
GREEN = \033[0;32m
BLUE = \033[0;34m
YELLOW = \033[1;33m
RED = \033[0;31m
RESET = \033[0m

all: $(NAME)

$(LIBFT):
	@echo "$(BLUE)📚 Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS_DIR) $(OBJS) $(GNL_OBJS)
	@echo "$(BLUE)🔗 Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(GNL_OBJS) $(LIBFT) -o $(NAME) $(MLX_LIB) $(LFLAGS)
	@echo "$(GREEN)✅ $(NAME) is ready! 🎮$(RESET)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@mkdir -p $(OBJS_DIR)/map
	@mkdir -p $(OBJS_DIR)/map_utils


$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c 
	@echo "$(YELLOW)🔨 Compiling $<...$(RESET)"
	@$(CC) -c $< -o $@

$(OBJS_DIR)/map/%.o: $(MAP_DIR)/%.c
	@echo "$(YELLOW)🔨 Compiling MAP $<...$(RESET)"
	@$(CC) -c $< -o $@

$(OBJS_DIR)/map_utils/%.o: $(MAP_UTIL_DIR)/%.c
	@echo "$(YELLOW)🔨 Compiling MAP UTİLS $<...$(RESET)"
	@$(CC) -c $< -o $@

$(OBJS_DIR)/gnl/%.o: $(GNL_DIR)/%.c
	@echo "$(YELLOW)🔨 Compiling GNL $<...$(RESET)"
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@


clean:
	@echo "$(RED)🧹 Cleaning object files...$(RESET)"
	@make -C $(LIBFT_DIR) clean
	rm -rf $(OBJS_DIR)

fclean: clean
	@echo "$(RED)🗑️ Removing $(NAME)...$(RESET)"
	@make -C $(LIBFT_DIR) fclean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re



