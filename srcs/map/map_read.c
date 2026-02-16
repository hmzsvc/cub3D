/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 13:52:18 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/16 23:36:58 by hsyn             ###   ########.fr       */
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

static void read_cub(int fd)
{
	char	*line;
	char	*joined_map;
	char	*temp;
	t_game	*game;
	int		flag;
	int		newline_flag;

	game = global_game();
	joined_map = NULL;
	temp = NULL;
	line = get_next_line(fd);
	flag = 0;
	newline_flag = 0;

	while (line)
	{
		if (is_map_line(line))
			flag = 1;
		if (flag == 1 && *line == '\n')
			newline_flag = 1;
		if (flag == 1 && newline_flag == 1 && (map_newline_check(line))) // Hata yazdirilicaka ve exit atilacak (Burada line içeriğinde newline, boşluk ve tab harici bir şey varsa hata yazılacak)
			printf("HARITADA BOSLUK VAR!\n");
		temp = joined_map;
		joined_map = ft_strjoin(joined_map, line);
		if (temp)
			free(temp);
		free(line);
		line = get_next_line(fd);
	}

	game->all_line = ft_split(joined_map, '\n');
	//for (size_t i = 0; game->all_line[i]; i++)
	//{
	//	printf("ALL_LINES[%d]: $%s$\n", i, game->all_line[i]);
	//}
	
}

char *skip_whitespaces(char *line)
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
	int		len;
	
	if (!str)
		return (NULL);
	len = ft_strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r'))
	{
		str[len - 1] = '\0';
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
	if (!trimmed)
	{
		printf("Element path not found\n");
		exit(1);
	}
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

static void	player_check_dir(char *line, int map_y) // Burada player x ve y almak için çift boyutlu diziye geçmek lazım
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
		{
			set_player_dir(3 * (PI / 2));
			game->player.x = (index * BLOCK) + (BLOCK / 2);
			game->player.y = (map_y * BLOCK) + (BLOCK / 2);
		}
		else if (trimmed[index] == 'S')
		{
			set_player_dir(PI / 2);
			game->player.x = (index * BLOCK) + (BLOCK / 2);
			game->player.y = (map_y * BLOCK) + (BLOCK / 2);
		}
		else if (trimmed[index] == 'W')
		{
			set_player_dir(PI);
			game->player.x = (index * BLOCK) + (BLOCK / 2);
			game->player.y = (map_y * BLOCK) + (BLOCK / 2);
		}
		else if (trimmed[index] == 'E')
		{
			set_player_dir(0);
			game->player.x = (index * BLOCK) + (BLOCK / 2);
			game->player.y = (map_y * BLOCK) + (BLOCK / 2);
		}
		index++;
	}
}

int	is_map_line(char *line)
{
	int		i;
	//char	*trimmed;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	
	if (line[i] == '1' || line[i] == '0')
		return (1);
	return (0);

	//trimmed = skip_whitespaces(line);	
	//if (*trimmed == '1' || *trimmed == '0')
	//{
	//	return (1);
	//}
	//return (0);
}

int	is_empty_line(char *line)
{
	char *trimmed;	
	
	trimmed = skip_whitespaces(line);
	if (*trimmed == '\0' || *trimmed == '\n')
		return (1);
	return (0);
}

int	map_space_check(char *line)
{
	char	*trimmed;

	trimmed = skip_whitespaces(line);
	if (*trimmed == '\n')
		return (1);
	return (0);
}

static int	count_map_lines() // Map started olduğunda map line count artacak fakat is_empty ile new line kontrolü yaparak map arasında boşluk varmı check eklenecek 
{
	char	**line;
	int		line_count;
	int		map_started;
	int		index;
	t_game *game;

	game = global_game();
	line_count = 0;
	map_started = 0;
	//line = game->all_line;
	index = 0;
	while (game->all_line[index])
	{
		if (is_map_line(game->all_line[index]))
		{
			map_started = 1;
			line_count++;
		}
		else if (map_started == 1 && !is_map_line(game->all_line[index]) && map_space_check(game->all_line[index]))
			printf("HATAAAASDASDASDSADASDASAAAA\n");;
		//else if (map_started && !is_empty_line(line) && is_map_line(line))
		//	break;
		//free(*line);
		index++;
	}
	//while (*line)
	//{
	//	free(*line);
	//	*line++;
	//}
	//printf("LINE_COUNT: $%d$\n", line_count);
	return (line_count);
}

static char **read_map_util(int	line_count)
{
	t_game	*game;
	char	**map;
	int		i;
	int		index;

	game = global_game();
	map	= calloc(sizeof(char *), line_count + 1);
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
			// printf("map[%d]: %s\n", i, map[i]);
			i++;
		}
		//free(*line);
		index++;
	}
	if (game->player.dir_check == 0)
	{
		printf("Player Yok!\n");
		
		exit(1);
	}
	map[i] = NULL;
	return (map);
}

static int	parse_util(t_game *game)
{
	char	**line;
	int		count;
	int		index;

	count = 0;
	index = 0;
	//line = game->all_line;
	while (game->all_line[index] && count < 6)
	{
		if (!is_empty_line(game->all_line[index]) && !is_map_line(game->all_line[index]))
		{
			if (parse_element(game->all_line[index]))
				count++;
		}
		else if (is_map_line(game->all_line[index]))
		{
			//free(game->all_line[index]);
			break;
		}
		//free(*line);
		index++;
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
	read_cub(fd);
	close(fd);
	game->map_element_count = parse_util(game);
	if (game->map_element_count != 6) // Hata kontrolü & Hata mesajı:(Map elements err)
	{
		printf("Map element found\n");
		exit(1);
	}
	game->map_lines_count = count_map_lines();
	if (game->map_lines_count == 0) // BURASI TEKRAR KONTROL EDİLECEK
		return (1);
	game->map = read_map_util(game->map_lines_count);
	set_map_dimension();
	wall_control();
	//game->map_clone = read_map_util(fd, game->map_lines_count);
	return (0);
}