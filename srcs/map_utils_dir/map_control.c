/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:09:51 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/20 03:47:34 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

// Map'in üst kısmı için y = 0 x++ kontrol edilecek
// Map'in sol sütun kısmı için x = 0 y++ kontrol edilecek
// Map'in en sağ sütun kısmı için x max value y++ kontrol edilecek
// map'in alt kısmı için y max value x++ kontrol edilecek
static void	unkown_character_check(char c)
{
	int	map_component;
	int	player_component;
	int	whitespace_component;
	
	map_component = (c != '1' && c != '0');
	player_component = (c != 'S' && c != 'N' && c != 'E' && c != 'W');
	whitespace_component = (c != ' ' && c != '\t');
	if (map_component && player_component && whitespace_component)
	{
		printf("Unkown Character Error!\n");
		clear_garbage();
		exit (1);
	}
	
}

void	wall_control_util(int x, int y) // Burada map[y]'de boşluk varsa boşluktan bir önceki wall'mu kontrol edilecek
{
	int		is_wall;
	t_game 	*game;

	game = global_game();
	if (!game->map[y][x])
	{
		printf("Map Yok\n");
		clear_garbage();
		exit(1);	
	}
	is_wall = (y == 0  || x == 0 || y == game->map_height - 1 || x == ft_strlen(game->map[y]) - 1);
	if (is_wall && game->map[y][x] != ' ' && game->map[y][x] != '\t' && game->map[y][x] != '1')
	{
		printf("MAP DUVAR HATA!  x: %d - y: %d\n", x, y);
		clear_garbage();
		exit(1);
	}
}

static void	flood_fill(int x, int y)
{
	t_game	*game;

	game = global_game();
	if (x < 0 || y < 0 || !game->map_clone[y] || !game->map_clone[y][x])
	{
		printf("Map Hata1\n");
		game->error_code = 1;
		clear_garbage();
		exit(1);
	}
	if (game->map_clone[y][x] == '1' || game->map_clone[y][x] == 'V')
		return ;
	if (game->map_clone[y][x] != '1' && game->map_clone[y][x] != '0' && game->map_clone[y][x] != 'V' && game->map_clone[y][x] != 'S'
		&& game->map_clone[y][x] != 'N' && game->map_clone[y][x] != 'E' && game->map_clone[y][x] != 'W')
	{
		printf("Map Hata2\n");
		game->error_code = 2;
		clear_garbage();
		exit(1);
	}
	game->map_clone[y][x] = 'V';

	flood_fill(x - 1, y);
	flood_fill(x + 1, y);
	flood_fill(x, y - 1);
	flood_fill(x, y + 1);
}

static void	create_map_clone()
{
	t_game	*game;
	int		y;

	game = global_game();
	y = 0;
	game->map_clone = ft_calloc(sizeof(char *), game->map_height + 1);
	if (!game->map_clone)
	{
		printf("Map Clone Create Error!\n");
		clear_garbage();
		exit (1);
	}
	while (game->map[y])
	{
		game->map_clone[y] = ft_strdup(game->map[y]);
		if (!game->map_clone[y])
        {
            printf("Map Clone Strdup Error!\n");
			clear_garbage();
            exit (1); ;
        }
		y++;
	}
	game->map_clone[y] = NULL;
}

void	wall_control()
{
	t_game	*game;
	int		x;
	int		y;

	game = global_game();
	y = 0;
	create_map_clone();
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			wall_control_util(x, y);
			unkown_character_check(game->map[y][x]);
			x++;
		}
		game->map_width = x;
		y++;	
	}
	flood_fill((int)(game->player.x / BLOCK), (int)(game->player.y / BLOCK));

}
