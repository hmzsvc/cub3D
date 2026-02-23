/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 20:59:30 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/23 22:46:11 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

char	*trim_newline(char *str)
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

int	parse_element(char *line)
{
	t_game	*game;
	char	*path;
	char	*trimmed;

	game = global_game();
	trimmed = skip_whitespaces(line);
	if (!trimmed)
	{
		//printf("Element path not found\n");
		//exit(1);
		error_handle("Element path not found");
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
	t_game	*game;

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

int	parse_util(t_game *game)
{
	char	**line;
	int		count;
	int		index;

	count = 0;
	index = 0;
	while (game->all_line[index])
	{
		if (!is_empty_line(game->all_line[index])
			&& !is_map_line(game->all_line[index]))
		{
			if (parse_element(game->all_line[index]))
				count++;
		}
		else if (is_map_line(game->all_line[index]))
		{
			break ;
		}
		index++;
	}
	return (count);
}
