/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:44:42 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/24 15:50:02 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

static void	unkown_character_check(char c)
{
	int		map_component;
	int		player_component;
	int		whitespace_component;
	t_game	*game;

	game = global_game();
	map_component = (c != '1' && c != '0');
	player_component = (c != 'S' && c != 'N' && c != 'E' && c != 'W');
	whitespace_component = (c != ' ' && c != '\t');
	if (map_component && player_component && whitespace_component)
		error_handle("Unkown character error");
}

void	wall_control_util(int x, int y)
{
	int		is_wall;
	t_game	*game;

	game = global_game();
	if (!game->map[y][x])
		error_handle("Map not found");
	is_wall = (y == 0 || x == 0 || y == game->map_height - 1
			|| x == ft_strlen(game->map[y]) - 1);
	if (is_wall && game->map[y][x] != ' ' && game->map[y][x] != '\t'
		&& game->map[y][x] != '1')
		error_handle("Map wall error");
	wall_control_continue(x, y);
}

static void	flood_fill(int x, int y)
{
	t_game	*game;

	game = global_game();
	if (x < 0 || y < 0 || !game->map_clone[y] || !game->map_clone[y][x])
		error_handle("Map error");
	if (game->map_clone[y][x] == '1' || game->map_clone[y][x] == 'V')
		return ;
	if (game->map_clone[y][x] != '1' && game->map_clone[y][x] != '0'
		&& game->map_clone[y][x] != 'V' && game->map_clone[y][x] != 'S'
		&& game->map_clone[y][x] != 'N' && game->map_clone[y][x] != 'E'
		&& game->map_clone[y][x] != 'W')
		error_handle("Map invalid character");
	game->map_clone[y][x] = 'V';
	flood_fill(x - 1, y);
	flood_fill(x + 1, y);
	flood_fill(x, y - 1);
	flood_fill(x, y + 1);
}

void	create_map_clone(void)
{
	t_game	*game;
	int		y;

	game = global_game();
	y = 0;
	game->map_clone = ft_calloc(sizeof(char *), game->map_height + 1);
	if (!game->map_clone)
	{
		error_handle("Map clone error");
	}
	while (game->map[y])
	{
		game->map_clone[y] = ft_strdup(game->map[y]);
		if (!game->map_clone[y])
			error_handle("Map clone strdup error");
		y++;
	}
	game->map_clone[y] = NULL;
}

void	wall_control(void)
{
	t_game	*game;
	int		x;
	int		y;

	game = global_game();
	y = 0;
	flood_fill((int)(game->player.x / BLOCK), (int)(game->player.y / BLOCK));
	while (game->map[y])
	{
		x = 0;
		while (game->map[y][x])
		{
			if (game->map[y][x] == '\t')
				error_handle("Tab error");
			wall_control_util(x, y);
			unkown_character_check(game->map[y][x]);
			x++;
		}
		y++;
	}
}
