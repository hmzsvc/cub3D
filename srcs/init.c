/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsyn <hsyn@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/23 23:57:27 by hsyn              #+#    #+#             */
/*   Updated: 2026/02/23 23:58:36 by hsyn             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/game.h"

void	init_player(t_player *player)
{
	player->x = WIDTH / 2;
	player->y = HEIGHT / 2;
	player->angle = 2 * PI;
	player->key_up = false;
	player->key_down = false;
	player->key_right = false;
	player->key_left = false;
	player->left_rotate = false;
	player->right_rotate = false;
	player->dir_check = 0;
}

void	init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		error_handle("Mlx init fail");
	game->map_element_count = 0;
	game->map_lines_count = 0;
	game->map_flag = 0;
	game->newline_flag = 0;
	game->gap_check = 0;
}

t_game	*global_game(void)
{
	static t_game	*game;

	if (!game)
		game = (t_game *)ft_calloc(1, sizeof(t_game));
	if (!game)
		error_handle("Game could not be created");
	return (game);
}