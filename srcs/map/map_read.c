/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:52:18 by hsyn              #+#    #+#             */
/*   Updated: 2025/12/09 18:39:08 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"
#include <unistd.h>
#include <fcntl.h>

static int	open_map(char *map_path)
{
	int	fd;

	fd = open(map_path, O_RDONLY);
	if (!fd)
	{

		perror("Fd cannot open!");
		return (0);
	}
	return (fd);
}

static char *read_map_utility(int fd)
{
	char	*line;
	char	*temp;
	char	*joined_map;

	joined_map = NULL;
	line = get_next_line(fd);
	while (line)
	{
		temp = joined_map;
		joined_map = ft_strjoin(joined_map, line);
		if (temp)
			free(temp);
		free(line);
		line = get_next_line(fd);
	}
	return (joined_map);
}

char	**read_map(char *map_path, t_game *game)
{
	int		fd;
	char	**map;
	char	*map_line;

	fd = open_map(map_path);
	if (fd <= 0)
		return (NULL);

	map_line = read_map_utility(fd);
	close(fd);
	if (!map_line)
		return (NULL);
	map =  ft_split(map_line, '\n');
	free(map_line);
	return (map);
}