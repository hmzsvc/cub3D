/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:52:18 by hsyn              #+#    #+#             */
/*   Updated: 2025/12/06 16:02:44 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"
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
	
}

char	**read(char *map_path, t_game *game)
{
	int		fd;
	char	**map;
	char	*map_line;
	
	fd = open_file(map_path);
	if (fd <= 0)
		return (NULL);
	map_line = read_map_utility(fd);
	
}