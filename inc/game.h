#ifndef GAME_H
#define GAME_H

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;
} t_game;

// Function declarations
void init_game(t_game *game);

#endif