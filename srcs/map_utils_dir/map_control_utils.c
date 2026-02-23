/* ***************************************************************************************** */
/*                                                                                           */
/*                                                                       :::      ::::::::   */
/*   map_control_utils.c                                               :+:      :+:    :+:   */
/*                                                                   +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42kocaeli.com.tr>              +#+  +:+       +#+        */
/*                                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 13:25:08 by huozturk                         #+#    #+#             */
/*   Updated: 2026/02/23 15:03:08 by huozturk                        ###   ########.tr       */
/*                                                                                           */
/* ***************************************************************************************** */

#include "../../inc/game.h"

int	map_newline_check(char *line)
{
	int	i;

	i = 0;
	while (line[i] && (line[i] == ' ' || line[i] == '\t' || line[i] == '\n'))
		i++;
	if (line[i])
		return (1);
	return (0);
}

void	map_gap_check(char *line)
{
	t_game	*game;
	int		newline_flag;

	game = global_game();
	newline_flag = 0;
	if (is_map_line(line))
		game->map_flag = 1;
	if (game->map_flag == 1 && !is_map_line(line)
		&& (*line == '\n' || *line == ' ' || *line == '\t'))
		game->newline_flag = 1;
	if (game->map_flag == 1 && game->newline_flag == 1 && is_map_line(line))
		game->gap_check = 1;
}
