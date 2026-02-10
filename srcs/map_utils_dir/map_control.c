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

void	wall_control()
{
	t_game	*game;
	int		x;
	int		y;

	game = global_game();
	y = 0;
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
}

// Map'in ortasında da boşluk olabilir ve çevresi wall olmak zorunda ayrı kontrol lazım ('0' çevresinde boşluk karakteri varsa hata dönülebilir kısa yol)
// Map'de istenilmeyen karakter kontorlü burada yapılabilir bir flag olacak (is_wall) gibi olması gerek karakter harici karakter gelirse '1' olacak ve hata dönecek