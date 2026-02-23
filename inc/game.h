#ifndef GAME_H
#define GAME_H

#define WIDTH 1280
#define HEIGHT 768
#define BLOCK 64
#define DEBUG 0

#define W 119
#define A 97
#define S 115
#define D 100
#define LEFT 65361
#define RIGHT 65363

#define PI 3.14159265359

// Minimap ayarları - SABİT ÖLÇEK
#define MINIMAP_SCALE 5         // 1/5 oranında küçültme (BLOCK/5 = 12.8 pixel)
#define MINIMAP_X 20            // Sol üst köşeden X uzaklığı
#define MINIMAP_Y 20            // Sol üst köşeden Y uzaklığı
#define MINIMAP_BORDER 3        // Kenar kalınlığı
// Karakterin çarpışma yarıçapı (BLOCK'tan küçük olmalı)
#define COLLISION_RADIUS 10 


#include "../minilibx-linux/mlx.h"
#include "../lib/get_next_line/get_next_line.h"
#include "../lib/libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct s_game t_game;
typedef struct s_minimap
{
    int	pos_x;
    int	pos_y;
    int	scale;
}   t_minimap;

typedef struct s_draw
{
    int     h;
    int     start;
    int     end;
    float   step;
    float   tex_pos;
    int     tex_x;
    int     tex_y;
}	t_draw;


typedef struct s_ray
{
    float   angle;
    int     map_x;
    int     map_y;
    float   side_x;
    float   side_y;
    float   delta_x;
    float   delta_y;
    int     step_x;
    int     step_y;
    int     side;
    float   wall_dist;
    float   wall_x;
}   t_ray;

typedef struct s_player
{
    float	x;
    float	y;
    float	angle;
	int		dir_check;
    bool	key_up;
    bool	key_down;
    bool	key_left;
    bool	key_right;
    bool	left_rotate;
    bool	right_rotate;
    t_game	*game;
}	t_player;

typedef struct s_texture
{
	void	*img;
	char	*addr;
	int		width;
	int		height;
	int		bpp;
	int		line_len;
	int		endian;
}	t_texture;


typedef struct s_game
{
	int			map_element_count;
	int			map_lines_count;
	int			cub_lines_count;
	int			error_code;
    void		*mlx;
    void		*win;
    void		*img;
    char		*data;
    int			bbp;
    int			size_line;
    int			endian;
	char		**all_line;
    char		**map;
	int			map_width;
	int			map_height;
    char		**map_clone;
    t_player	player;
    t_minimap	minimap;
	t_texture	n_tex;
	t_texture	s_tex;
	t_texture	e_tex;
	t_texture	w_tex;
	char		*n_path;
	char		*s_path;
	char		*e_path;
	char		*w_path;
	int			floor_color;
	int			ceiling_color;

	int			newline_flag;
	int			map_flag;
	int			gap_check;

}	t_game;

void	init_minimap(t_minimap *minimap);
void	draw_minimap(t_game *game);
void	init_game(t_game *game);
void	init_player(t_player *player);
int		key_release(int keycode, t_player *player);
int		key_press(int keycode, t_player *player);
void	move_player(t_player *player);
bool	touch(float px, float py, t_game *game);
float	distance(float x, float y);
float	fixed_dist(float x1, float y1, float x2, float y2, t_game *game);
void	draw_line(t_player *player, t_game *game, float start_x, int i);
void	render_frame(t_game *game);
void	put_pixel(int x, int y, int color, t_game *game);
void		close_game(t_game *game);
int		success_close_game(t_game *game);
t_game	*global_game();
void	read_map(char *map_path);
int		is_empty_line(char *line);
int		is_map_line(char *line);
int		parse_element(char *line);
int		parse_floor_ceiling(char *trimmed);
int		parse_element_continue(char *trimmed);
void	set_player_dir(double angle, int index, int map_y);
char	*skip_whitespaces(char *line);
void	wall_control();
void	set_map_dimension();
int		map_newline_check(char *line);
void	extension_control(char *map_argv);
void	load_all_tex();
int		get_tex_pixel(t_texture	*tex, int x, int y);
void	wall_control_continue(int x, int y);
void	invalid_character_check(char *line);
void	map_gap_check(char *line);
int		map_space_check(char *line);
int		parse_color(char *line);
char	*trim_newline(char *str);
int		parse_util(t_game *game);
void	read_cub(char *path);
int		open_map(char *map_path);
void	create_map_clone();





#endif