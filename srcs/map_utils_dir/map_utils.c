/* ***************************************************************************************** */
/*                                                                                           */
/*                                                                       :::      ::::::::   */
/*   map_utils.c                                                       :+:      :+:    :+:   */
/*                                                                   +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42kocaeli.com.tr>              +#+  +:+       +#+        */
/*                                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:25:37 by huozturk                         #+#    #+#             */
/*   Updated: 2026/02/23 15:13:22 by huozturk                        ###   ########.tr       */
/*                                                                                           */
/* ***************************************************************************************** */

#include "../../inc/game.h"

void	set_player_dir(double angle, int index, int map_y)
{
	t_game	*game;

	game = global_game();
	if (game->player.dir_check != 0)
	{
		printf("Player Dir ERROR\n");
		close_game(game);
	}
	else
		game->player.dir_check = 1;
	game->player.angle = angle;
	game->player.x = (index * BLOCK) + (BLOCK / 2);
	game->player.y = (map_y * BLOCK) + (BLOCK / 2);
}

void	set_map_dimension(void)
{
	int		y;
	t_game	*game;

	y = 0;
	game = global_game();
	while (game->map[y])
		y++;
	game->map_height = y;
}

void	wall_control_continue(int x, int y)
{
	t_game	*game;

	game = global_game();
	if (game->map[y][x] == '0' && (game->map[y][x + 1] == ' '
		|| game->map[y][x - 1] == ' '
		|| game->map[y + 1][x] == ' ' || game->map[y - 1][x] == ' '))
	{
		printf("MAP Wall Error!\n");
		close_game(game);
	}
	if (game->map[y][x] == '0' && (game->map[y][x + 1] == '\0'
		|| game->map[y][x - 1] == '\0'
		|| game->map[y + 1][x] == '\0' || game->map[y - 1][x] == '\0'))
	{
		printf("MAP Wall Error!\n");
		close_game(game);
	}
}

void	invalid_character_check(char *line)
{
	int	tex_check;
	int	ceil_check;
	int	map_check;
	int	whitespaces_check;

	tex_check = *line != 'S' && *line != 'N' && *line != 'W' && *line != 'E';
	ceil_check = *line != 'F' && *line != 'C';
	map_check = *line != '1' && *line != '0';
	whitespaces_check = *line != ' ' && *line != '\t' && *line != '\0';
	if (tex_check && ceil_check && map_check && whitespaces_check)
	{
		printf("Invalid Character\n");
		exit(1);
	}	
}

char	*skip_whitespaces(char *line)
{
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	return (line);
}
