/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_read_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasivaci <hasivaci@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 16:44:50 by hasivaci          #+#    #+#             */
/*   Updated: 2026/02/23 16:44:54 by hasivaci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

int	is_empty_line(char *line)
{
	char	*trimmed;	

	trimmed = skip_whitespaces(line);
	if (*trimmed == '\0' || *trimmed == '\n')
		return (1);
	return (0);
}

int	is_map_line(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '1' || line[i] == '0')
		return (1);
	return (0);
}

int	map_space_check(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t'))
		i++;
	if (line[i] == '\n' || line[i] == '\0')
		return (0);
	return (1);
}

static int	comma_check(char *line)
{
	int	i;
	int	comma_count;

	i = 0;
	comma_count = 0;
	while (line[i])
	{
		if (line[i] == ',')
			comma_count++;
		i++;
	}
	return (comma_count);
}

int	parse_color(char *line)
{
	int		r;
	int		g;
	int		b;
	char	**split;
	t_game	*game;

	game = global_game();
	if (comma_check(line) != 2)
		error_handle("Comma error");
	split = ft_split(line, ',');
	if (!split || !split[0] || !split[1] || !split[2])
		return (-1);
	r = ft_atoi(split[0]);
	g = ft_atoi(split[1]);
	b = ft_atoi(split[2]);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	return ((r << 16) | (g << 8) | b);
}
