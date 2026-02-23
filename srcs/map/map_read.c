/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:52:18 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/23 23:48:12 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

static void	player_check_dir(char *line, int map_y)
{
	int		index;
	char	*trimmed;
	t_game	*game;

	index = 0;
	game = global_game();
	trimmed = skip_whitespaces(line);
	while (trimmed[index])
	{
		if (trimmed[index] == 'N')
			set_player_dir(3 * (PI / 2), index, map_y);
		else if (trimmed[index] == 'S')
			set_player_dir(PI / 2, index, map_y);
		else if (trimmed[index] == 'W')
			set_player_dir(PI, index, map_y);
		else if (trimmed[index] == 'E')
			set_player_dir(0, index, map_y);
		index++;
	}
}

static int	count_map_lines(t_game	*game)
{
	int		line_count;
	int		map_started;
	int		index;

	line_count = 0;
	map_started = 0;
	index = 0;
	while (game->all_line[index])
	{
		invalid_character_check(game->all_line[index]);
		if (is_map_line(game->all_line[index]))
		{
			map_started = 1;
			line_count++;
		}
		if (map_started == 1 && !is_map_line(game->all_line[index])
			&& map_space_check(game->all_line[index]))
			error_handle("Invalid Format");
		index++;
	}
	return (line_count);
}

static char	**read_map_util(int line_count)
{
	t_game	*game;
	char	**map;
	int		i;
	int		index;

	game = global_game();
	map = ft_calloc(sizeof(char *), line_count + 1);
	if (!map)
		return (NULL);
	i = 0;
	index = 0;
	while (game->all_line[index] && i < line_count)
	{	
		if (is_map_line(game->all_line[index]))
		{
			player_check_dir(game->all_line[index], i);
			map[i] = trim_newline(ft_strdup(game->all_line[index]));
			i++;
		}
		index++;
	}
	map[i] = NULL;
	return (map);
}

void	read_map(char *path)
{
	t_game	*game;

	game = global_game();
	read_cub(path);
	game->map_element_count = parse_util(game);
	if (game->map_element_count != 6)
		error_handle("Map element not found");
	game->map_lines_count = count_map_lines(game);
	if (game->gap_check == 1)
		error_handle("Gap error");
	game->map = read_map_util(game->map_lines_count);
	if (game->player.dir_check == 0)
		error_handle("Player not found");
	set_map_dimension();
	create_map_clone();
	wall_control();
}
