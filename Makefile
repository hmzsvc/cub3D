NAME = game
CC = cc
SRC = srcs/main.c
OBJ = $(SRC:.c=.o)

LFLAGS = -lmlx -lXext -lX11 -lm -lz

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) $(LFLAGS)

%.o: %.c
	$(CC) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re