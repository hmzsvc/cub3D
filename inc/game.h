/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 17:01:18 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/24 16:17:58 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GAME_H
# define GAME_H

# define WIDTH 1280
# define HEIGHT 768
# define BLOCK 64
# define DEBUG 0

# define W 119
# define A 97
# define S 115
# define D 100
# define LEFT 65361
# define RIGHT 65363

# define PI 3.14159265359

# define COLLISION_RADIUS 10

# include "../lib/get_next_line/get_next_line.h"
# include "../lib/libft/libft.h"
# include "../minilibx-linux/mlx.h"
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>

typedef struct s_game	t_game;

typedef struct s_draw
{
	int					h;
	int					start;
	int					end;
	float				step;
	float				tex_pos;
	int					tex_x;
	int					tex_y;
}						t_draw;

typedef struct s_ray
{
	float				angle;
	int					map_x;
	int					map_y;
	float				side_x;
	float				side_y;
	float				delta_x;
	float				delta_y;
	int					step_x;
	int					step_y;
	int					side;
	float				wall_dist;
	float				wall_x;
}						t_ray;

typedef struct s_player
{
	float				x;
	float				y;
	float				angle;
	int					dir_check;
	bool				key_up;
	bool				key_down;
	bool				key_left;
	bool				key_right;
	bool				left_rotate;
	bool				right_rotate;
	t_game				*game;
}						t_player;

typedef struct s_texture
{
	void				*img;
	char				*addr;
	int					width;
	int					height;
	int					bpp;
	int					line_len;
	int					endian;
}						t_texture;

typedef struct s_game
{
	void				*mlx;
	void				*win;
	void				*img;
	int					map_element_count;
	int					map_lines_count;
	int					bbp;
	int					size_line;
	int					endian;
	int					map_height;
	int					floor_color;
	int					ceiling_color;
	int					newline_flag;
	int					map_flag;
	int					gap_check;
	char				*data;
	char				**all_line;
	char				**map;
	char				**map_clone;
	char				*n_path;
	char				*s_path;
	char				*e_path;
	char				*w_path;
	t_player			player;
	t_texture			n_tex;
	t_texture			s_tex;
	t_texture			e_tex;
	t_texture			w_tex;
}						t_game;

void					init_game(t_game *game);
void					init_player(t_player *player);
int						key_release(int keycode, t_player *player);
int						key_press(int keycode, t_player *player);
void					move_player(t_player *player);
void					render_frame(t_game *game);
void					put_pixel(int x, int y, int color, t_game *game);
void					close_game(t_game *game);
int						success_close_game(t_game *game);
t_game					*global_game(void);
void					read_map(char *map_path);
int						is_empty_line(char *line);
int						is_map_line(char *line);
void					set_player_dir(double angle, int index, int map_y);
char					*skip_whitespaces(char *line);
void					wall_control(void);
void					set_map_dimension(void);
void					extension_control(char *map_argv);
void					load_all_tex(void);
int						get_tex_pixel(t_texture *tex, int x, int y);
void					wall_control_continue(int x, int y);
void					invalid_character_check(char *line);
void					map_gap_check(char *line);
int						map_space_check(char *line);
int						parse_color(char *line);
char					*trim_newline(char *str);
int						parse_util(t_game *game);
void					read_cub(char *path);
void					create_map_clone(void);
void					error_handle(char *err_str);
void					calc_wall_distance(t_ray *ray, t_game *g);
void					calc_wall_x(t_ray *ray, t_game *g);
t_texture				*select_texture(t_ray *ray, t_game *g);
void					ft_put_draw(t_draw *d, t_game *g, t_texture *tex,
							int x);
void					calc_draw_data(t_game *g, t_ray *ray, t_texture *tex,
							t_draw *draw);

#endif