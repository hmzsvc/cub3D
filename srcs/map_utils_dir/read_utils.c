/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:45:06 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/23 16:45:10 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <fcntl.h>
#include <unistd.h>

static int	open_map(char *map_path)
{
	t_game	*game;
	int		fd;

	game = global_game();
	fd = open(map_path, O_RDONLY);
	if (fd <= 0)
		error_handle("Fd cannot open");
	return (fd);
}

void	read_cub(char *path)
{
	char	*line;
	char	*joined_map;
	char	*temp;
	t_game	*game;
	int		fd;

	game = global_game();
	fd = open_map(path);
	if (fd <= 0)
		error_handle("Fd error");
	joined_map = NULL;
	temp = NULL;
	line = get_next_line(fd);
	while (line)
	{
		map_gap_check(line);
		temp = joined_map;
		joined_map = ft_strjoin(joined_map, line);
		line = get_next_line(fd);
	}
	game->all_line = ft_split(joined_map, '\n');
	close(fd);
}
