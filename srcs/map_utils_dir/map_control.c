/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:09:51 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/11 00:50:27 by hsyn             ###   ########.fr       */
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
		printf("Unkown Character Error!\n");
	
	//if (c != '1' && c != '0' && c != 'S' && c != 'N' && c != 'E' && c != 'W' && c != ' ')
	//	printf("Unkown Character Error!\n");
}

void	wall_control_util(int x, int y)
{
	int		is_wall;
	t_game 	*game;

	game = global_game();
	is_wall = (y == 0  || x == 0 || y == game->map_height - 1 || x == ft_strlen(game->map[y]) - 1);
	if (is_wall && game->map[y][x] != ' ' && game->map[y][x] != '\t' && game->map[y][x] != '1')
		printf("MAP DUVAR HATA!  x: %d - y: %d\n", x, y);
}

static void	flood_file(int x, int y)
{
	t_game	*game;

	game = global_game();
	if (x < 0 || y < 0 || !game->map_clone[y] || !game->map_clone[y][x])
	{
		printf("Map Hata1\n");
		game->error_code = 1;
		return ;
	}
	if (game->map_clone[y][x] == '1' || game->map_clone[y][x] == 'V')
		return ;
	if (game->map[y][x] != '1' && game->map[y][x] != '0' && game->map[y][x] != 'V' && game->map[y][x] != 'S'
		&& game->map[y][x] != 'N' && game->map[y][x] != 'E' && game->map[y][x] != 'W')
	{
		printf("Map Hata2\n");
		game->error_code = 2;
		return ;
	}
	game->map[y][x] = 'V';
	flood_file(x - 1, y);
	flood_file(x + 1, y);
	flood_file(x, y - 1);
	flood_file(x, y + 1);
}

static void	create_map_clone()
{
	t_game	*game;
	int		y;

	game = global_game();
	y = 0;
	game->map_clone = calloc(sizeof(char *), game->map_height + 1);
	if (!game->map_clone) // Burada Error Func'a gönderilecek
		printf("Map Clone Create Error!\n");
	while (game->map[y])
	{
		game->map_clone[y] = game->map[y];
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
	flood_file((int)(game->player.x / BLOCK), (int)(game->player.y / BLOCK));
	printf("Player_x: %f - Player_y: %f\n", game->player.x / BLOCK, game->player.y);
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			// flood_file(x, y);
			wall_control_util(x, y);
			unkown_character_check(game->map[y][x]);
			x++;
		}
		game->map_width = x;
		y++;	
	}
}

// Map'in ortasında da boşluk olabilir ve çevresi wall olmak zorunda ayrı kontrol lazım ('0' çevresinde boşluk karakteri varsa hata dönülebilir kısa yol) (flood_fill) 
// Map'de istenilmeyen karakter kontorlü burada yapılabilir bir flag olacak (is_wall) gibi olması gerek karakter harici karakter gelirse '1' olacak ve hata dönecek

// Map clone için her line'ı stdup ile bir diziye atıp her atıldığında da map_clone[y] ekleyebiliriz map_clone ise map_height kadar yer tahsis edilecek