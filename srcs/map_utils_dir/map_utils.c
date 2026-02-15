/* ***************************************************************************************** */
/*                                                                                           */
/*                                                                       :::      ::::::::   */
/*   map_utils.c                                                       :+:      :+:    :+:   */
/*                                                                   +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42kocaeli.com.tr>              +#+  +:+       +#+        */
/*                                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:25:37 by huozturk                         #+#    #+#             */
/*   Updated: 2026/02/06 20:52:09 by huozturk                        ###   ########.tr       */
/*                                                                                           */
/* ***************************************************************************************** */

#include "../../inc/game.h"

void	set_player_dir(double angle)
{
	t_game *game;

	game = global_game();
	if (game->player.dir_check != 0) // Burada hata yazdırıp exit atıcaz
		printf("Player Dir ERROR\n");
	else
		game->player.dir_check = 1;
	game->player.angle = angle;
}

void	set_map_dimension()
{
	//int		x;
	int		y;
	t_game	*game;

	y = 0;
	//x = 0;
	game = global_game();
	while (game->map[y])
	{
		y++;
	}
	game->map_height = y;
	//while (game->map[x])
	//{
	//	x++;
	//}
	//game->map_width = x;
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

	result = calloc(ft_strlen(line) + 1, sizeof(char));
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