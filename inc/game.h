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
#include "../lib/get_next_line/get_next_line.h"
#include "../lib/libft/libft.h"
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
}	t_player;

typedef struct s_texture
{
	void	*img;		//img objesi
	char	*addr;		//Pixel data address
	int		width;		//Img Width
	int		height;		//Img Height
	int		bpp;		//Bits per pixel (pixel başına bit sayısı) (Pixelin biti yani rgba her biri 8 bit r:8 g:8 b:8 a:8)
	int		line_len;	//Satır uzunluğu
	int		endian;		//Byte sıralaması

} t_texture;


typedef struct s_game
{
	int	map_element_count; //MAP ELEMENTS COUNT
	int	map_lines_count; //MAP LİNES COUNT


    void *mlx;
    void *win;
    void *img;

    char *data;
    int bbp;
    int size_line;
    int endian;
    
    char **map;

    t_player player;


	// TEXTURE VARİABLES
	t_texture	n_tex;
	t_texture	s_tex;
	t_texture	e_tex;
	t_texture	w_tex;

	char		*n_path;
	char		*s_path;
	char		*e_path;
	char		*w_path;


	// COLOR VARİABLES
	int			floor_color;
	int			ceiling_color;

}	t_game;

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

//	Game Func
t_game *global_game();





//	-------------- MAP -------------- 
//char	**read_map(char *map_path, t_game *game);
int	read_map(char *map_path);
static int	parse_util(int fd, t_game *game);
static char **read_map_util(int	fd,	int	line_count);
static int	count_map_lines(int fd);
int	is_empty_line(char *line);
int	is_map_line(char *line);
int	parse_floor_ceiling(char *trimmed);
int	parse_element_continue(char *trimmed);
//int	parse_element(char *trimmed);
//static char *trim_newline(char *str);
//static int	parse_color(char *line);
char *whitespaces_term(char *line);






//	-------------- TEX --------------
int	load_all_tex();
int	get_tex_pixel(t_texture	*tex, int x, int y);

#endif