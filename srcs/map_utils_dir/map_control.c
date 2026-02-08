/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/07 16:09:51 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/08 18:36:11 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/game.h"

// Map'in üst kısmı için y = 0 x++ kontrol edilecek
// Map'in sol sütun kısmı için x = 0 y++ kontrol edilecek
// Map'in en sağ sütun kısmı için x max value y++ kontrol edilecek
// map'in alt kısmı için y max value x++ kontrol edilecek
void	wall_control_util(int x, int y)
{
	int		is_wall;
	t_game 	*game;

	game = global_game();
	//is_wall = (y == 0 || x == 0 || x == (game->map))	
	
}

void	wall_control(const char *line)
{
	t_game	*game;
	int		x;
	int		y;

	game = global_game();
	y = 0;
	x = 0;
	while (game->map_clone[y])
	{
		while (game->map_clone[y][x])
		{
			wall_control_util(x, y);
			x++;
		}
		y++;	
	}
}