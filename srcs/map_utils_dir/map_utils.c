/* ***************************************************************************************** */
/*                                                                                           */
/*                                                                       :::      ::::::::   */
/*   map.utils.c                                                       :+:      :+:    :+:   */
/*                                                                   +:+ +:+         +:+     */
/*   By: huozturk <huozturk@student.42kocaeli.com.tr>              +#+  +:+       +#+        */
/*                                                               +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 18:25:37 by huozturk                         #+#    #+#             */
/*   Updated: 2026/02/06 18:31:43 by huozturk                        ###   ########.tr       */
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