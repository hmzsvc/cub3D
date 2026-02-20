/* ***************************************************************************************** */
/*                                                                                           */
/*                                                                       :::      ::::::::   */
/*   map_utils.c                                                       :+:      :+:    :+:   */
/*                                                                   +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42kocaeli.com.tr>              +#+  +:+       +#+        */
/*                                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:25:37 by huozturk                         #+#    #+#             */
/*   Updated: 2026/02/20 17:16:06 by huozturk                        ###   ########.tr       */
/*                                                                                           */
/* ***************************************************************************************** */

#include "../../inc/game.h"

void	set_player_dir(double angle)
{
	t_game *game;

	game = global_game();
	if (game->player.dir_check != 0)
	{
		printf("Player Dir ERROR\n");
		close_game(game);
		exit(1);
	}
	else
		game->player.dir_check = 1;
	game->player.angle = angle;
}

void	set_map_dimension()
{
	int		y;
	t_game	*game;

	y = 0;
	game = global_game();
	while (game->map[y])
	{
		y++;
	}
	game->map_height = y;

}

char *whitespaces_term(char *line)
{
	int	i;
	int	j;
	char	*result;

	i = 0;
	j = 0;
	if (!line)
		return (NULL);	

	result = ft_calloc(ft_strlen(line) + 1, sizeof(char));
	if (!result)
		return (NULL);
	
	while (line[j])
	{
		if (line[j] != ' ' && line[j] != '\t' && line[j] != '\n')
		{
			result[i] = line[j];
			i++;
		}
		j++;

	}
	result[i] = '\0';
	return (result);
}
