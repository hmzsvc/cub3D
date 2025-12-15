#ifndef GAME_H
#define GAME_H

#define WIDTH 1280
#define HEIGHT 720
#define BLOCK 64
#define DEBUG 0

#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT 65361
#define RIGHT 65363

#define PI 3.14159265359

#include "../minilibx-linux/mlx.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct s_game t_game;

typedef struct s_player
{
    float x;
    float y;
    float angle;

    bool key_up;
    bool key_down;
    bool key_left;
    bool key_right;
    
    bool left_rotate;
    bool right_rotate;

    t_game *game;
}t_player;

typedef struct s_game
{
    void *mlx;
    void *win;
    void *img;

    char *data;
    int bbp;
    int size_line;
    int endian;
    
    char **map;

    t_player player;
} t_game;

// Function declarations
void init_game(t_game *game);
void init_player(t_player *player);
int key_release(int keycode, t_player *player);
int key_press(int keycode, t_player *player);
void move_player(t_player *player);

// Raycast functions
bool touch(float px, float py, t_game *game);
float distance(float x, float y);
float fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
void draw_line(t_player *player, t_game *game, float start_x, int i);
void perform_raycasting(t_game *game);
void put_pixel(int x, int y, int color, t_game *game);
int close_game(t_game *game);


#endif