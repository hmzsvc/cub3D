/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:52:18 by hsyn              #+#    #+#             */
/*   Updated: 2026/01/25 19:09:32 by hsyn             ###   ########.fr       */
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


static char *skip_whitespaces(char *line)
{
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	return (line);
}

static int	parse_color(char *line)
{
	int		r;
	int		g;
	int		b;
	char	**split;
	int		i;
	
	split = ft_split(line, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (-1);
	r = ft_atoi(split[0]); // 8 bit
	g = ft_atoi(split[1]); // 8 bit
	b = ft_atoi(split[2]); // 8 bit toplam 24bit rgb
	i = 0;
	while (split[i])
		free(split[i++]);
	free(split);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}

static char *trim_newline(char *str)
{
	int	len;
	
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] == '\0';
		len--;
	}
	return (str);
}

static int parse_element(char *line)
{
	t_game	*game;
	char	*path;
	char	*trimmed;

	game = global_game();
	trimmed = skip_whitespaces(line);
	if (ft_strncmp(trimmed, "NO ", 3) == 0)
	{
		path = trim_newline(skip_whitespaces(trimmed + 3));
		game->n_path = ft_strdup(path);
		return (1);
	}
	if (ft_strncmp(trimmed, "SO ", 3) == 0)
	{
		path = trim_newline(skip_whitespaces(trimmed + 3));
		game->s_path = ft_strdup(path);
		return (1);
	}
	return (parse_element_continue(trimmed));
}

int	parse_element_continue(char *trimmed)
{
	t_game	*game;
	char	*path;

	game = global_game();
	if (ft_strncmp(trimmed, "WE ", 3) == 0)
	{
		path = trim_newline(skip_whitespaces(trimmed + 3));
		game->w_path = ft_strdup(path);
		return (1);
	}
	if (ft_strncmp(trimmed, "EA ", 3) == 0)
	{
		path = trim_newline(skip_whitespaces(trimmed + 3));
		game->e_path = ft_strdup(path);
		return (1);
	}
	return (parse_floor_ceiling(trimmed));
}

int	parse_floor_ceiling(char *trimmed)
{
	t_game *game;

	game = global_game();
	if (ft_strncmp(trimmed, "F ", 2) == 0)
	{
		game->floor_color = parse_color(skip_whitespaces(trimmed + 2));
		if (game->floor_color == -1)
			return (0);
		return (1);
	}
	if (ft_strncmp(trimmed, "C ", 2) == 0)
	{
		game->ceiling_color = parse_color(skip_whitespaces(trimmed + 2));
		if (game->ceiling_color == -1)
			return (0);
		return (1);
	}
	return (0);
}

int	is_map_line(char *line)
{
	char	*trimmed;

	trimmed = skip_whitespaces(line);
	if (*trimmed == '1' ||*trimmed == '0')
		return (1);
	return (0);
}

int	is_empty_line(char *line)
{
	char *trimmed;	
	
	trimmed = skip_whitespaces(line);
	if (*trimmed == '\0' || *trimmed == '\n')
		return (1);
	return (0);
}

static int	count_map_lines(int fd)
{
	char	*line;
	int		line_count;
	int		map_started;

	line_count = 0;
	map_started = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (is_map_line(line))
		{
			map_started = 1;
			line_count++;
		}
		else if (map_started && !is_empty_line(line))
			break;
		free(line);
		line = get_next_line(fd);
	}
	while (line)
	{
		free(line);
		line = get_next_line(fd);
	}
	return (line_count);
}

static char **read_map_util(int	fd,	int	line_count)
{
	char	**map;
	char	*line;
	int		i;

	map	= calloc(sizeof(char *), line_count + 1);
	if (!map)
		return (NULL);
	i = 0;
	line = get_next_line(fd);
	while (line && i < line_count)
	{
		if (is_map_line(line))
		{
			map[i] = trim_newline(ft_strdup(line));
			i++;
		}
		free(line);
		line = get_next_line(fd);
	}
	map[i] = NULL;
	return (map);
}

static int	parse_util(int fd, t_game *game)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(fd);
	while (line && count < 6)
	{
		if (!is_empty_line(line) && !is_map_line(line))
		{
			if (parse_element(line))
				count++;
		}
		else if (is_map_line(line))
		{
			free(line);
			break;
		}
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}

int read_map(char *path)
{
	int		fd;
	t_game	*game;

	game = global_game();
	fd = open_map(path);
	if (fd < 0)
		return (1);
	game->map_element_count = parse_util(fd, game);
	close(fd);
	if (game->map_element_count != 6) // Hata kontrolü & Hata mesajı:(Map elements err)
		return (1);
	fd = open_map(path);
	game->map_lines_count = count_map_lines(fd);
	close(fd);
	if (game->map_lines_count == 0) // BURASI TEKRAR KONTROL EDİLECEK
		return (1);
	fd = open_map(path);
	game->map = read_map_util(fd, game->map_lines_count);

	int	f = 0;
	while (f >= 0)
	{
		printf("MAP[%d]: $%s$\n", f, game->map[1]);
		f--;
	}
	
	close (fd);
	return (0);
}




//				OLD

//static int	open_map(char *map_path)
//{
//	int	fd;

//	fd = open(map_path, O_RDONLY);
//	if (!fd)
//	{

//		perror("Fd cannot open!");
//		return (0);
//	}
//	return (fd);
//}

//static char *read_map_utility(int fd)
//{
//	char	*line;
//	char	*temp;
//	char	*joined_map;

//	joined_map = NULL;
//	line = get_next_line(fd);
//	while (line)
//	{
//		temp = joined_map;
//		joined_map = ft_strjoin(joined_map, line);
//		if (temp)
//			free(temp);
//		free(line);
//		line = get_next_line(fd);
//	}
//	return (joined_map);
//}

//char	**read_map(char *map_path, t_game *game)
//{
//	int		fd;
//	char	**map;
//	char	*map_line;

//	fd = open_map(map_path);
//	if (fd <= 0)
//		return (NULL);

//	map_line = read_map_utility(fd);
//	close(fd);
//	if (!map_line)
//		return (NULL);
//	map =  ft_split(map_line, '\n');
//	free(map_line);
//	return (map);
//}